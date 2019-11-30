// Copyright (C) 2019 Aleksey Khachaturov
// https://github.com/triadium/MDHD
// License: GNU GPL 3 or later, see file LICENSE

#pragma once

namespace Game {

	// Процессор проверки столкновения игровых объектов.
	// В большинстве случаев обновление состояния и проверка
	// осуществлются не каждый кадр, а по таймеру, но для
	// упрощения расчет производится с пропуском заданного
	// количества кадров.

	class PhysicProcessor: public Processor
	{
	public:
		PhysicProcessor(Scene * scene);
		void Update(float dt) override;
		void RemovePendingObjects(const Object::Container & objectsToBeRemoved) override;
		void Clear() override;

	private:
		void CheckIntersects(const FRect& boundsA, const FRect& boundsB, Object* objA, Object* objB);
		void CheckCollisions(Object* objA, Object* objB);

	private:
		// Направленная связь между двумя объектами
		typedef std::pair<Object*, Object*> ObjectLinkKey;
		typedef std::unordered_map<ObjectLinkKey, bool, boost::hash<ObjectLinkKey>> ObjectLinkContainer;
		// Множество как набор "флажков" для объектов
		typedef std::unordered_set<Object*> ObjectSet;
		typedef std::vector <ObjectLinkKey> ObjectLinkKeyVector;

		// Направленная связь между двумя коллайдерами
		typedef std::pair<Collider*, Collider*> ColliderLinkKey;
		typedef std::unordered_set <ColliderLinkKey, boost::hash<ColliderLinkKey>> ColliderLinkContainer;

		int _skipFrameCount;
		int _skipedFrameCount;
		ObjectLinkContainer _boundIntersections;
		ColliderLinkContainer _collisions;
		ObjectSet _isCollidersUpdatedSet;
	};
}
