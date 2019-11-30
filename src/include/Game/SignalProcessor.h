// Copyright (C) 2019 Aleksey Khachaturov
// https://github.com/triadium/MDHD
// License: GNU GPL 3 or later, see file LICENSE

#pragma once

namespace Game {

	// Процессор обработки очереди сигналов.
	// Очередь сигналов хранится внутри сцены
	// и через сцену можно публиковать и 
	// подписываться на сигналы
	// FIXME: На данный момент принято решение использовать очередь 
	// сообщений слоя и оповещать все виджеты, сцена взяла на себя
	// роль контроллера

	class SignalProcessor: public Processor
	{
	public:
		SignalProcessor(Scene * scene);
		void Update(float dt) override;
		void RemovePendingObjects(const Object::Container & objectsToBeRemoved) override;
		void Clear() override;
	};
}
