// Copyright (C) 2019 Aleksey Khachaturov
// https://github.com/triadium/MDHD
// License: GNU GPL 3 or later, see file LICENSE

#pragma once

namespace Game {

	class Scene;
	
	// Процессор сцены обновляет ее состояние путем обработки
	// всех переданных объектов сцены. Возможно, лучше всего
	// определять интерфейсы и через dynamic_cast при инциализации
	// сцены, а так же при добавлении/удалении обновлять списки
	// объектов, которые могут участвовать в обработке именно данным
	// процессором (например, декор сцены не нужно обрабатывать 
	// системой определения столкновений) Но придется использовать RTTI. 
	// Для этого можно добавить функции фильтрации для объекта,
	// которые будет вызывать сцена при добавлении объекта
	class Processor
	{
	public:
		typedef std::unique_ptr<Processor> Unique;
		typedef std::vector<Unique> Container;

		Processor(Scene * scene);

		virtual void Update(float dt) = 0;
		// Необходимо очистить все ссылки на объекты, которые должны быть удалены со сцены
		virtual void RemovePendingObjects(const Object::Container & objectsToBeRemoved) = 0;
		virtual void Clear() = 0;

	protected:
		Scene * _scene;
	};
}
