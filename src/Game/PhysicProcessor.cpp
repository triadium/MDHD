#include "stdafx.h"
#include <unordered_set>
#include <unordered_map>
#include "Game/Collider.h"
#include "Game/Object.h"
#include "Game/Scene.h"
#include "Game/Processor.h"
#include "Game/PhysicProcessor.h"

namespace Game {
	
	// �������� ������ ������������ ���������� ��������� ����������� "��������������� �����", 
	// ���� ������ � ����� �� ������� � ����� "�����" � ���������� ��� �������� ����������. 
	// "��������������� �����" �� ���� �������� AABB, ����������� ������� ����� ������ ����������� �������. 
	// ���� � ���� �������� ���� ����������� AABB � ������ ����������� ��� � ��������� ���������� �����������,
	// �� ��� ������ �������� ���������� ������� ����������� � ����������� AABB ��� ������� ����� �����������.  
	// � ����� ������ ���� ��� ������� �����������: 
	// 1) ���� (������ ��� �����������/�����������), 
	// 2) ���������� (�����������/ ������������ ������������), 
	// 3) ����� (�����������/������������ ������������).
	// ��� ��������� ����� �������������� ���� ������� "����", �� ����� ����� ��������� � �� ������� ������.
	// ���� ���� �� ���� �� ���� ������� ������� "����", �� ����� ����������� �� ����������� �� ����������. 
	// ���� ���� �������� �� ������������, �� ������� �� ������� �� ��������� ���������� �����������.
	// ��� ������ ����������� ����������� �������� �������� ��������� ������� �������� � ��������� ����������
	// ������ ����������� � ��� ���������� "������" ������� �������� ������. ����� ��� ������ ���� �����������
	// �������� ��������� ������������ � �������� ������� ���� ��� "�����", � ��� ������ "������" ��������
	// ������� ���� ����������� �� ��������� ���������� ������������.
	// ��� ��� ������ ���������� �������������� ������� �� ��������� �����, �� ����� � ���������� � ������ ��
	// ���� ��������, ���� �� ������� ������ ��� ��� ����� ���������� �� ��� ��� ���� �� ����� ������ �����
	// ������ ��������� ����������.


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
						// ����� ���������� ������� ��� ����������� "��������������� �����" �������
						// ������� ��������
						if (collidersA.size() > 0 || boundsA.Width() > 0) {

							for (auto itB = itA + 1; itB != objects.cend(); ++itB) {
								auto objB = itB->get();
								if (objB->IsActive()) {
									auto& collidersB = objB->GetColliders();
									auto& boundsB = objB->GetAabb();
									if (collidersB.size() > 0 || boundsB.Width() > 0) {
										CheckIntersects(boundsA, boundsB, objA, objB);
										// ���� ����� ������ �������� ���� ��������, �� ������ �� ���������
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
					// ��������� ������ ���� �� ����������
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

	// FIXME: ����� ������ �� ������������� �������� ��������� �������,
	// ������� ���������� � ��������� ������������� � ��������� 
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

						// ���� ����� ���� �� �������� �������� ���� ��������,
						// �� ������ �� ��������� ������������
						// ����� ��� ����, ����� �� �������������� ������������ ��� ����,
						// ���� ����� ��� ��������� � ����� ������ � ��������� ������� ���������,
						// �� ��� ������� �������� - ��� ���������.
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