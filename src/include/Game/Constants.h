// Copyright (C) 2019 Aleksey Khachaturov
// https://github.com/triadium/MDHD
// License: GNU GPL 3 or later, see file LICENSE

#pragma once

namespace Game {

	namespace Names {
		// FIXME: Может быть отделить имена уровня слоя от имен уровня игры?
		// Или лучше делить константы на файлы со своим набором пространств
		extern const std::string Scene;
		extern const std::string IntroLayer;
		extern const std::string IntroScene;
		extern const std::string StageLayer;
		extern const std::string StageScene;
		extern const std::string StageSettingsFile;
		extern const std::string TimeParameter;
		extern const std::string CountTargetParameter;
		extern const std::string BulletSpeedParameter;
		extern const std::string DefeatTextParameter;
		extern const std::string VictoryTextParameter;

		extern const std::string Countdown;
		extern const std::string Wall;
		extern const std::string Target;
		extern const std::string Bomb;
		extern const std::string Head;
		extern const std::string Body;
		extern const std::string Armor;
		extern const std::string Gun;
		extern const std::string Tower75;
		extern const std::string Tower50;
		extern const std::string Tower25;
		extern const std::string Barrel;
		extern const std::string RedLamp;
		extern const std::string GreenLamp;
		extern const std::string Bullet;

		// Специальные детали игровых объектов
		extern const std::string Wing;
		extern const std::string AnchovyRadiance;

		// Цели
		extern const std::string Duck;
		extern const std::string Egg;
		extern const std::string Fish;
		extern const std::string Anchovy;
	}

}

enum {
	WINDOW_WIDTH = 1024,
	WINDOW_HEIGHT = 768
};
