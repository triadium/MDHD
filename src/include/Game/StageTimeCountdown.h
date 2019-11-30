// Copyright (C) 2019 Aleksey Khachaturov
// https://github.com/triadium/MDHD
// License: GNU GPL 3 or later, see file LICENSE

#pragma once

namespace Game {

	class StageTimeCountdown : public Object
	{
	public:
		typedef boost::intrusive_ptr<StageTimeCountdown> Ptr;
		typedef std::vector<Ptr> Container;

		// Модель данных "фабрики" целей
		struct Model {
			// Количество секунд для обратного отсчета
			float countdown;
			// Предупреждение, если обратный отсчет уже меньше данного времени
			float warningTime;
			// Множитель масштаба надписи со временем для предупреждения
			TimedSpline<float> scaleFactor;
		};

		StageTimeCountdown(const std::string& name, const std::string& fontName);

		void Draw() override;
		void Update(float dt) override;

		const Model & GetModel() const;
		void SetModel(const Model & value);

	private:
		void InternalUpdateData();

	private:		
		std::string _fontName;
		FPoint _offset;
		Model _model;
		float _t;
		float _wt;
		int _tickSoundId;
	};

}
