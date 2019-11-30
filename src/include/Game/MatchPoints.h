// Copyright (C) 2019 Aleksey Khachaturov
// https://github.com/triadium/MDHD
// License: GNU GPL 3 or later, see file LICENSE

#pragma once

namespace Game {
	
	// Объект подсчета и отображения набранных очков за один матч
	// FIXME: Необходимо организовать общее хранилище и тогда можно будет
	// создать уровни в виде отдельных сцен без сброса данных о набранных очках

	class MatchPoints : public Object
	{
	public:
		typedef boost::intrusive_ptr<MatchPoints> Ptr;
		typedef std::vector<Ptr> Container;
		
		// Модель данных "фабрики" целей
		struct Model {
			std::unordered_map<std::string, int> typePointsMap;
			// Множитель масштаба надписи со временем для показа изменения в виде импульса
			TimedSpline<float> scaleFactor;
		};

		MatchPoints(const std::string& name, const std::string& fontName);

		void Draw() override;
		void Update(float dt) override;

		const Model & GetModel() const;
		void SetModel(const Model & value);

		void increasePoints(const std::string& type);
		int GetPoints() const;

	private:
		void InternalUpdateData();

	private:		
		std::string _fontName;
		FPoint _offset;
		Model _model;
		float _t;
		// Количество полученных очков
		int _points;
	};

}
