// Copyright (C) 2019 Aleksey Khachaturov
// https://github.com/triadium/MDHD
// License: GNU GPL 3 or later, see file LICENSE

#pragma once

namespace Game {

	class TargetSpawner : public Object
	{
	public:
		typedef boost::intrusive_ptr<TargetSpawner> Ptr;
		typedef std::vector<Ptr> Container;

		// ћодель данных "фабрики" целей
		struct Model {
			// ћаксимальное количество целей на уровень от этой "фабрики"
			int maxCount;
			// ѕериод создани€ очередной цели
			float period;
		};

		TargetSpawner(const std::string& name, Target* prefab);

		void Update(float dt) override;

		const Model & GetModel() const;
		void SetModel(const Model & value);

	private:
		Target::Ptr Create();
		void InternalUpdateData();

	private:		
		Target::Ptr _prefab;
		Model _model;
		float _t;
		int _count;
	};

}
