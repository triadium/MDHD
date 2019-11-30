#include "stdafx.h"
#include <unordered_set>
#include <unordered_map>
#include "Game/Collider.h"
#include "Game/Object.h"
#include "Game/Scene.h"
#include "Game/Processor.h"
#include "Game/PhysicProcessor.h"

namespace Game {
	
	// Алгоритм поиска столкновений изначально проверяет пересечение "ограничительных рамок", 
	// если ширина и длина не нулевые у обоих "рамок" и коллайдеры для объектов определены. 
	// "Ограничительная рамка" по сути является AABB, описывающая границы всего набора коллайдеров объекта. 
	// Если у пары объектов есть пересечение AABB и такого пересечения нет в множестве предыдущих пересечений,
	// то для данных объектов вызывается функции уведомления о пересечении AABB без расчета точек пересечения.  
	// В общем случае есть три функции уведомления: 
	// 1) Вход (только что пересеклись/столкнулись), 
	// 2) Активность (пересечение/ столкновение продолжается), 
	// 3) Выход (пересечение/столкновение прекратилось).
	// Для упрощения будет использоваться пока функция "Вход", но потом можно расширить и до полного набора.
	// Если хотя бы одна из двух функций вернула "ложь", то поиск пересечения по коллайдерам не производим. 
	// Если пара объектов не пересекается, то пробуем ее удалить из множества предыдущих пересечений.
	// При поиске пересечения коллайдеров объектов алгоритм проверяет наличие объектов в множестве обновлений
	// данных коллайдеров и при отсутствии "просит" объекты обновить данные. Затем для каждой пары коллайдеров
	// объектов проверяет столкновение и вызывает функцию лишь для "Входа", а для случая "Выхода" пытается
	// удалить пару коллайдеров из множества предыдущих столкновений.
	// Так как сейчас отсуствуют уведомительные функции об изменении сцены, то ключи в множествах и картах из
	// пары объектов, один из которых удален или оба будут оставаться до тех пор пока не будет вызван метод
	// сброса состояния процессора.


	PhysicProcessor::PhysicProcessor(Scene * scene)
		: Processor(scene)
		, _skipFrameCount(0)
		, _skipedFrameCount(0)
	{
	}

	void PhysicProcessor::Update(float dt)
	{			
		if (_skipedFrameCount <= 0) {
			_skipedFrameCount = _skipFrameCount;

			auto& objects = _scene->GetObjects();
			if (objects.size() > 1) {

				for (auto itA = objects.cbegin(); itA != objects.cend(); ++itA) {
					auto objA = itA->get();

					if (objA->IsActive()) {

						auto& collidersA = objA->GetColliders();
						auto& boundsA = objA->GetAabb();
						// Можем установить объекту без коллайдеров "ограничительную рамку" вручную
						// Простая проверка
						if (collidersA.size() > 0 || boundsA.Width() > 0) {

							for (auto itB = itA + 1; itB != objects.cend(); ++itB) {
								auto objB = itB->get();
								if (objB->IsActive()) {
									auto& collidersB = objB->GetColliders();
									auto& boundsB = objB->GetAabb();
									if (collidersB.size() > 0 || boundsB.Width() > 0) {
										CheckIntersects(boundsA, boundsB, objA, objB);
										// Если вдруг объект перестал быть активным, то дальше не проверяем
										if (!objA->IsActive()) { break; }
									}
									// else { skip }
								}
								// else { skip }
							}
						}
						// else { skip }
					}
					// else { skip }
				}

				_isCollidersUpdatedSet.clear();
			}
			// else { skip }
		}
		else {
			_skipedFrameCount--;
		}
		
	}

	void PhysicProcessor::RemovePendingObjects(const Object::Container & objectsToBeRemoved)
	{
		ObjectLinkKeyVector keysToBeRemoved;
	
		for (auto& obj : objectsToBeRemoved) {
			for (auto& kvBi : _boundIntersections) {
				if (kvBi.first.first == obj || kvBi.first.second == obj) {
					// Добавляем только одну из комбинаций
					keysToBeRemoved.push_back(kvBi.first);
					break;
				}
				//else { skip }
			}
		}

		if (keysToBeRemoved.size() > 0) {
			for (auto& key : keysToBeRemoved) {
				for (auto& uniqA : key.first->GetColliders()) {
					auto colliderA = uniqA.get();
					for (auto& uniqB : key.second->GetColliders()) {
						auto colliderB = uniqB.get();
						auto keyAB = std::make_pair(colliderA, colliderB);
						auto keyBA = std::make_pair(colliderB, colliderA);
						_collisions.erase(keyAB);
						_collisions.erase(keyBA);
					}
				}
				auto invKey = std::make_pair(key.second, key.first);
				_boundIntersections.erase(key);
				_boundIntersections.erase(invKey);
			}
		}
		// else { skip }
	}


	void PhysicProcessor::Clear()
	{
		_skipedFrameCount = 0;
		_boundIntersections.clear();
		_collisions.clear();
		_isCollidersUpdatedSet.clear();
	}

	void PhysicProcessor::CheckIntersects(const FRect& boundsA, const FRect& boundsB, Object * objA, Object * objB)
	{
		auto keyAB = std::make_pair(objA, objB);
		if (boundsA.Intersects(boundsB)) {
			auto bit = _boundIntersections.find(keyAB);
			if (bit == _boundIntersections.end()) {
				// Enter

				bool checkCollisionsA = objA->OnEnterBoundsIntersect(objB);
				bool checkCollisionsB = objB->OnEnterBoundsIntersect(objA);

				auto keyBA = std::make_pair(objB, objA);
				bool checkCollisions = checkCollisionsA && checkCollisionsB;
				_boundIntersections.insert_or_assign(keyAB, checkCollisions);
				_boundIntersections.insert_or_assign(keyBA, checkCollisions);

				if (checkCollisions) {
					CheckCollisions(objA, objB);
				}
				// else { skip }
			}
			else {
				// Stay 
				if (bit->second) {
					CheckCollisions(objA, objB);
				}
				// else { skip }
			}
		}
		else {
			// Exit?
			auto keyBA = std::make_pair(objB, objA);
			_boundIntersections.erase(keyAB);
			_boundIntersections.erase(keyBA);
		}
	}

	// FIXME: После первой же положительной проверки оповещаем объекты,
	// заносим коллайдеры в множество столкнувшихся и завершаем 
	void PhysicProcessor::CheckCollisions(Object * objA, Object * objB)
	{
		bool updateA = _isCollidersUpdatedSet.find(objA) == _isCollidersUpdatedSet.end();
		bool updateB = _isCollidersUpdatedSet.find(objB) == _isCollidersUpdatedSet.end();

		if (updateA) {
			objA->UpdateColliders();
			_isCollidersUpdatedSet.insert(objA);
		}
		// else { skip }

		if (updateB) {
			objB->UpdateColliders();
			_isCollidersUpdatedSet.insert(objB);
		}
		// else { skip }

		for (auto& uniqA : objA->GetColliders()) {
			auto colliderA = uniqA.get();
			for (auto& uniqB : objB->GetColliders()) {
				auto colliderB = uniqB.get();
				auto keyAB = std::make_pair(colliderA, colliderB);
				if (colliderA->CollidesWith(*colliderB)) {
					if (_collisions.find(keyAB) == _collisions.end()) {
						// Enter
						objA->OnEnterCollision(colliderA, objB, colliderB);
						objB->OnEnterCollision(colliderB, objA, colliderA);

						auto keyBA = std::make_pair(colliderB, colliderA);
						_collisions.insert(keyAB);
						_collisions.insert(keyBA);

						// Если вдруг один из объектов перестал быть активным,
						// то дальше не проверяем столкновения
						// Нужно для того, чтобы не обрабатывались столкновения два раза,
						// хотя можно это перенести в общем случае в обработку события объектами,
						// но для текущей ситуации - это нормально.
						if (!objA->IsActive() || !objB->IsActive()) { return; }
					}
					// else { Stay }
				}
				else {
					// Exit?
					auto keyBA = std::make_pair(colliderB, colliderA);
					_collisions.erase(keyAB);
					_collisions.erase(keyBA);
				}
			}
		}
	}

}