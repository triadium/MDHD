// Copyright (C) 2019 Aleksey Khachaturov
// https://github.com/triadium/MDHD
// License: GNU GPL 3 or later, see file LICENSE

#pragma once

namespace Game {

	// Процессор обновляет состояние объектов сцены
	// и обновляет состояние контейнера эффектов

	class UpdateProcessor : public Processor
	{
	public:
		UpdateProcessor(Scene * scene);
		void Update(float dt) override;
		void RemovePendingObjects(const Object::Container & objectsToBeRemoved) override;
		void Clear() override;
	};
}
