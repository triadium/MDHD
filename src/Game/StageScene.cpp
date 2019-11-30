#include "stdafx.h"
#include <regex>
#include <unordered_map>
#include "Game/Scene.h"
#include "Game/Object.h"
#include "Game/Processor.h"
#include "Game/UpdateProcessor.h"
#include "Game/PhysicProcessor.h"
#include "Game/SignalProcessor.h"

#include "Game/EggBomb.h"
#include "Game/Target.h"
#include "Game/DuckTarget.h"
#include "Game/FishTarget.h"
#include "Game/AnchovyTarget.h"

#include "Game/StageScene.h"

#include "Game/Constants.h"
#include "Game/TargetSpawner.h"
#include "Game/Gun.h"
#include "Game/Material.h"
#include "Game/BoxCollider.h"
#include "Game/CircleCollider.h"
#include "Game/View.h"
#include "Game/StageTimeCountdown.h"
#include "Game/StageInfoText.h"
#include "Game/MatchPoints.h"

namespace Game {

	StageScene::StageScene(const std::string & name, rapidxml::xml_node<>* elem)
		: Scene(name, elem)
		, _battleTrackId(-1)
		, _tShowRestartButton(-1)
		, _showRestartButton(false)
	{
		Init();
	}

	void StageScene::AcceptMessage(const Message & message)
	{
		const std::string& publisher = message.getPublisher();
		const std::string& data = message.getData();	
		if (publisher == "Layer") {
			if (data == "LayerInit") {
				_restartButton = getChild("RestartButton");
				_restartButton->setVisible(false);

				LoadSettings();
				Restart();
			}
			// else { skip }
		}
		else if ((publisher == Names::Countdown && data == "done") ||
			publisher == Names::Gun && data == "gun_destroyed" ) {
			Clear();
			MM::manager.StopAll();
			MM::manager.PlayTrack("defeat");

			StageInfoText::Ptr info = new StageInfoText("infotext", "bellflower");
			StageInfoText::Model infoModel;
			infoModel.text = "{font color=0;32;255;255;}" + _settings.getString(Names::DefeatTextParameter);
			infoModel.startPoint = FPoint(0, height * 0.5f);
			infoModel.endPoint = FPoint(width * 0.5f, height * 0.5f);
			infoModel.time = 0.5f;
			infoModel.showTime = INT_MAX;
			info->SetModel(infoModel);
			AddObject(info);

			StageInfoText::Ptr ptsInfo = new StageInfoText("ptstext", "bellflower");
			StageInfoText::Model ptsInfoModel;
			ptsInfoModel.text = "{font color=0;32;255;255;}PTS: " + utils::lexical_cast(reinterpret_cast<MatchPoints*>(_matchPoints.get())->GetPoints());
			ptsInfoModel.startPoint = FPoint(width * 0.5f, height * 1.5f);
			ptsInfoModel.endPoint = FPoint(width * 0.5f, height * 0.5f - 100);
			ptsInfoModel.time = 0.5f;
			ptsInfoModel.showTime = INT_MAX;
			ptsInfo->SetModel(ptsInfoModel);
			AddObject(ptsInfo);

			// TODO: Сделать настройки для количества взрывов на экране с текстом о поражении
			// Или поробовать сделать через привязку в редакторе эффектов
			for (int i = 0; i < 4; ++i) {
				float x = math::random(width * 0.3f, width * 0.7f);
				float y = math::random(height * 0.3f, height * 0.7f);
				MM::manager.PlaySample("boom");
				AddEffect("Explosion", FPoint(x, y));
			}

			_showRestartButton = true;
			_tShowRestartButton = 3.0f;
		}
		else if (publisher == Names::Target && data == "destroyed") {
			const std::string type = message.variables.getString("type");
			// FIXME: SIGNALS NEEDED!!!!
			reinterpret_cast<MatchPoints*>(_matchPoints.get())->increasePoints(data);

			if (type == Names::Duck) {
				_countTarget--;
				if (_countTarget <= 0) {
					MM::manager.FadeAll(0.5f);
					MM::manager.PlayTrack("victory");
					Clear();
					StageInfoText::Ptr info = new StageInfoText("infotext", "bellflower");
					StageInfoText::Model infoModel;
					infoModel.text = _settings.getString(Names::VictoryTextParameter);
					infoModel.startPoint = FPoint(width * 0.5f, height * 1.5f);
					infoModel.endPoint = FPoint(width * 0.5f, height * 0.5f);
					infoModel.time = 0.5f;
					infoModel.showTime = INT_MAX;
					info->SetModel(infoModel);
					AddObject(info);

					StageInfoText::Ptr ptsInfo = new StageInfoText("ptstext", "bellflower");
					StageInfoText::Model ptsInfoModel;
					ptsInfoModel.text = "PTS: " + utils::lexical_cast(reinterpret_cast<MatchPoints*>(_matchPoints.get())->GetPoints());
					ptsInfoModel.startPoint = FPoint(width * 0.5f, height * 1.5f);
					ptsInfoModel.endPoint = FPoint(width * 0.5f, height * 0.5f - 100);
					ptsInfoModel.time = 0.5f;
					ptsInfoModel.showTime = INT_MAX;
					ptsInfo->SetModel(ptsInfoModel);
					AddObject(ptsInfo);

					_showRestartButton = true;
					_tShowRestartButton = 3.0f;
				}
				// else { noop }
			}
		}
		else if (publisher == Names::Target && data == "damaged") {
			// FIXME: SIGNALS NEEDED!!!!
			reinterpret_cast<MatchPoints*>(_matchPoints.get())->increasePoints(data);
		}
		else if (publisher == Names::Target && data == "armor_destroyed") {
			// FIXME: SIGNALS NEEDED!!!!
			reinterpret_cast<MatchPoints*>(_matchPoints.get())->increasePoints(data);
		}
		else if (publisher == Names::Target && data == "armor_damaged") {
			// FIXME: SIGNALS NEEDED!!!!
			reinterpret_cast<MatchPoints*>(_matchPoints.get())->increasePoints(data);
		}
		else if (publisher == Names::Bullet && data == "headshot") {
			// FIXME: SIGNALS NEEDED!!!!
			reinterpret_cast<MatchPoints*>(_matchPoints.get())->increasePoints(data);
		}
		else if (publisher == Names::Bullet && data == "missed") {
			// FIXME: SIGNALS NEEDED!!!!
			reinterpret_cast<MatchPoints*>(_matchPoints.get())->increasePoints(data);
		}
		else if (publisher == Names::Bomb && data == "bomb_damaged") {
			// FIXME: SIGNALS NEEDED!!!!
			reinterpret_cast<MatchPoints*>(_matchPoints.get())->increasePoints(data);
		}
		else if (publisher == Names::Bomb && data == "ground_hit") {
			// FIXME: SIGNALS NEEDED!!!!
			reinterpret_cast<MatchPoints*>(_matchPoints.get())->increasePoints(data);
		}
		else if (publisher == Names::Gun && data == "gun_damaged") {
			// FIXME: SIGNALS NEEDED!!!!
			reinterpret_cast<MatchPoints*>(_matchPoints.get())->increasePoints(data);
		}
		else if (publisher == "RestartButton" && data == "press") {
			Restart();
		}
		// else { skip }
	}

	void StageScene::Update(float dt)
	{
#ifdef _DEBUG
		dt *= _dtFactor;
#endif // _DEBUG
		Scene::Update(dt);
		if (_showRestartButton) {
			if (_tShowRestartButton <= 0) {
				_showRestartButton = false;
				_tShowRestartButton = -1;
				_restartButton->setVisible(true);
			}
			else {
				_tShowRestartButton -= dt;
			}
		}
		// else { noop }
	}

#ifdef _DEBUG
	void StageScene::Draw()
	{
		Scene::Draw();
		if (Render::isFontLoaded("arial")) {
			Render::BindFont("arial");

			Render::PrintString(50, 50, "Press A to increase delta time factor, D to decrease: " + utils::lexical_cast(_dtFactor, 2));
		}
		// else { noop }
	}

	void StageScene::KeyPressed(int keyCode)
	{
		if (keyCode == VK_A) {
			_dtFactor += 0.1f;
		}
		else if (keyCode == VK_D) {
			_dtFactor -= 0.1f;
		}
		// else { noop }
		_dtFactor = math::clamp(0.0f, 2.0f, _dtFactor);
	}

#endif // _DEBUG

	void StageScene::Init()
	{
#ifdef _DEBUG
		_dtFactor = 1.0;
#endif // _DEBUG

		// Значения по-умолчанию для настроек уровня
		_settings.setInt(Names::TimeParameter, 60);
		_settings.setInt(Names::CountTargetParameter, 30);
		_settings.setInt(Names::BulletSpeedParameter, 390);
		_settings.setString(Names::DefeatTextParameter, "DEFEAT!");
		_settings.setString(Names::VictoryTextParameter, "VICTORY!");

		// TODO: Сделать инициализацию для сцены (процессоры и префабы) через xml файл
		
		AddProcessor(std::make_unique<UpdateProcessor>(this));
		AddProcessor(std::make_unique<PhysicProcessor>(this));
		AddProcessor(std::make_unique<SignalProcessor>(this));
	}

	FishTarget::Ptr StageScene::CreateFishPrefab(const Material::Ptr& baseMaterial)
	{
		// Рыбка-цель
		Target::Model model;
		model.hp = 25;
		model.ap = 0;
		model.speed = 30;
		model.power = 10;
		//
		model.speedFactor.addKey(0.0f, 0.3f);
		model.speedFactor.addKey(0.3f, 0.6f);
		model.speedFactor.addKey(0.7f, 0.3f);
		model.speedFactor.addKey(1.0f, 1.0f);
		//
		model.deviationX.addKey(0.0f, 0.0f);
		//
		model.deviationY.addKey(0.0f, 0.0f);
		model.deviationY.addKey(0.3f, 5.0f);
		model.deviationY.addKey(0.5f, 0.0f);
		model.deviationY.addKey(0.7f, -5.0f);
		model.deviationY.addKey(1.0f, 0.0f);

		FishTarget::Ptr target = new FishTarget(baseMaterial.get());
		target->AddCollider(FPoint(0.0f, 0.0f), std::make_unique<BoxCollider>(Names::Body, FPoint(50.0f, 30.0f)));
		// target->AddCollider(FPoint(0.15f, 0.15f), std::make_unique<CircleCollider>(Names::Head, 10.0f));

		target->SetModel(model);

		return target;
	}

	AnchovyTarget::Ptr StageScene::CreateAnchovyPrefab(const Material::Ptr& baseMaterial, const Material::Ptr& radianceMaterial)
	{
		// Рыбка-анчоус
		Target::Model model;
		model.hp = 25;
		model.ap = 0;
		model.speed = 30;
		model.power = 5;
		//
		model.speedFactor.addKey(0.0f, 0.0f);
		model.speedFactor.addKey(0.3f, 0.0f);
		model.speedFactor.addKey(0.7f, 0.0f);
		model.speedFactor.addKey(1.0f, 15.0f);
		//
		model.deviationX.addKey(0.0f, 0.0f);
		//
		model.deviationY.addKey(0.0f, 0.0f);
		model.deviationY.addKey(0.3f, 5.0f);
		model.deviationY.addKey(0.5f, 0.0f);
		model.deviationY.addKey(0.7f, -5.0f);
		model.deviationY.addKey(1.0f, 0.0f);

		AnchovyTarget::Ptr target = new AnchovyTarget(baseMaterial.get());
		target->AddCollider(FPoint(0.0f, 0.0f), std::make_unique<BoxCollider>(Names::Body, FPoint(50.0f, 30.0f)));
		// target->AddCollider(FPoint(0.15f, 0.15f), std::make_unique<CircleCollider>(Names::Head, 10.0f));

		View::Ptr radianceView = new View(Names::AnchovyRadiance, radianceMaterial.get());
		target->AddDecor(FPoint(0.0f, 0.0f), radianceView.get());
		
		target->SetModel(model);

		return target;
	}	

	EggBomb::Ptr StageScene::CreateEggBombPrefab(const Material::Ptr& baseMaterial)
	{
		// Обычное яйцо-бомба
		EggBomb::Model model;
		model.power = 25;
		model.gravityCoefficient = 1800.0f;

		EggBomb::Ptr bomb = new EggBomb(Names::Egg, Names::Bomb, baseMaterial.get());
		bomb->AddCollider(FPoint(0.0f, 0.0f), std::make_unique<CircleCollider>(Names::Egg, 12.0f));

		bomb->SetModel(model);

		return bomb;
	}

	Target::Ptr StageScene::CreateDuckPrefab(EggBomb::Ptr& eggBombPrefab, const Material::Ptr& baseMaterial, const Material::Ptr& wingMaterial)
	{
		// Обычная утка
		Target::Model model;
		model.hp = 100;
		model.ap = 0;
		model.speed = 150;
		//
		model.speedFactor.addKey(0.0f, 1.0f);
		model.speedFactor.addKey(0.3f, 0.6f);
		model.speedFactor.addKey(0.7f, 0.6f);
		model.speedFactor.addKey(1.0f, 1.0f);
		//
		model.deviationX.addKey(0.0f, 0.0f);
		//
		model.deviationY.addKey(0.0f, 0.0f);
		model.deviationY.addKey(0.3f, 15.0f);
		model.deviationY.addKey(0.5f, -15.0f);
		model.deviationY.addKey(0.7f, 15.0f);
		model.deviationY.addKey(1.0f, 0.0f);

		DuckTarget::Ptr target = new DuckTarget(eggBombPrefab.get(), baseMaterial.get());
		target->AddCollider(FPoint(0.0f, -0.27f), std::make_unique<BoxCollider>(Names::Body, FPoint(40.0f, 27.0f)));
		target->AddCollider(FPoint(0.15f, 0.15f), std::make_unique<CircleCollider>(Names::Head, 10.0f));

		View::Ptr wingView = new View(Names::Wing, wingMaterial.get());
		wingView->SetPivotOffset(0.186f, 0.0f);

		target->AddDecor(FPoint(0.05f, -0.24f), wingView.get());

		target->SetModel(model);

		return target;
	}

	Target::Ptr StageScene::CreateArmoredDuckPrefab(EggBomb::Ptr& eggBombPrefab, const Material::Ptr& baseMaterial, const Material::Ptr& wingMaterial, const Material::Ptr& armorMaterial)
	{
		// Бронированная утка
		Target::Model model;
		model.hp = 100;
		model.ap = 50;
		model.speed = 50;
		//
		model.speedFactor.addKey(0.0f, 1.0f);
		model.speedFactor.addKey(0.3f, 0.6f);
		model.speedFactor.addKey(0.7f, 0.6f);
		model.speedFactor.addKey(1.0f, 1.0f);
		//
		model.deviationX.addKey(0.0f, 0.0f);
		//
		model.deviationY.addKey(0.0f, 0.0f);
		model.deviationY.addKey(0.3f, 15.0f);
		model.deviationY.addKey(0.5f, -15.0f);
		model.deviationY.addKey(0.7f, 15.0f);
		model.deviationY.addKey(1.0f, 0.0f);

		DuckTarget::Ptr target = new DuckTarget(eggBombPrefab.get(), baseMaterial.get());

		target->AddCollider(FPoint(0.0f, -0.27f), std::make_unique<BoxCollider>(Names::Body, FPoint(40.0f, 27.0f)));
		target->AddCollider(FPoint(0.15f, 0.15f), std::make_unique<CircleCollider>(Names::Head, 10.0f));

		View::Ptr armorView = new View(Names::Armor, armorMaterial.get());

		View::Ptr wingView = new View(Names::Wing, wingMaterial.get());
		wingView->SetPivotOffset(0.186f, 0.0f);

		target->AddDecor(FPoint(-0.071f, -0.22f), armorView.get());
		target->AddDecor(FPoint(0.05f, -0.24f), wingView.get());

		target->SetModel(model);

		return target;
	}

	void StageScene::AddTargetSpawner(const FPoint & position, const Target::Ptr& prefab, const TargetSpawner::Model& model)
	{
		TargetSpawner::Ptr spawner = new TargetSpawner("spawner", prefab.get());
		spawner->SetModel(model);
		spawner->SetPosition(position);
		AddObject(spawner);
	}

	void StageScene::AddDuckSpawners()
	{
		// Префабы
		// Утки
		// ... из которых выпадают яйца
		// ... из которых вылупляются рыбки :)
		auto duckBaseTexture = Core::resourceManager.Get<Render::Texture>("duck_base");
		auto duckArmorTexture = Core::resourceManager.Get<Render::Texture>("duck_armor");
		auto duckWingTexture = Core::resourceManager.Get<Render::Texture>("duck_wing");
		auto eggBombTexture = Core::resourceManager.Get<Render::Texture>("egg_bomb");
		auto fishBaseTexture = Core::resourceManager.Get<Render::Texture>("fish_base");
		auto anchovyBaseTexture = Core::resourceManager.Get<Render::Texture>("anchovy_base");
		auto anchovyRadianceTexture = Core::resourceManager.Get<Render::Texture>("anchovy_radiance");

		Material::Ptr duckBaseMaterial = new Material(duckBaseTexture);
		Material::Ptr duckArmorMaterial = new Material(duckArmorTexture);
		Material::Ptr duckWingMaterial = new Material(duckWingTexture);
		Material::Ptr eggBombMaterial = new Material(eggBombTexture);
		Material::Ptr fishBaseMaterial = new Material(fishBaseTexture);
		Material::Ptr anchovyBaseMaterial = new Material(anchovyBaseTexture);
		Material::Ptr anchovyRadianceMaterial = new Material(anchovyRadianceTexture);

		auto fishPrefab = CreateFishPrefab(fishBaseMaterial);
		auto eggBombPrefab = CreateEggBombPrefab(eggBombMaterial);
		auto anchovyPrefab = CreateAnchovyPrefab(anchovyBaseMaterial, anchovyRadianceMaterial);
		eggBombPrefab->AddHatchPrefab(fishPrefab.get());
		eggBombPrefab->AddHatchPrefab(anchovyPrefab.get());
		auto duckPrefab = CreateDuckPrefab(eggBombPrefab, duckBaseMaterial, duckWingMaterial);
		auto armoredDuckPrefab = CreateArmoredDuckPrefab(eggBombPrefab, duckBaseMaterial, duckWingMaterial, duckArmorMaterial);

		int countTarget = _settings.getInt(Names::CountTargetParameter);
		int time = _settings.getInt(Names::TimeParameter);
		if (countTarget > 0 && time > 0) {

			TargetSpawner::Model spawnerModel;
			spawnerModel.maxCount = (countTarget - (countTarget % 3))/3;
			spawnerModel.period = time / 40.0f;
			
			AddTargetSpawner(FPoint(900.0f, 600.0f), duckPrefab, spawnerModel);

			spawnerModel.period = time / 10.0f;
			AddTargetSpawner(FPoint(100.0f, 700.0f), armoredDuckPrefab, spawnerModel);
			AddTargetSpawner(FPoint(100.0f, 500.0f), armoredDuckPrefab, spawnerModel);

			spawnerModel.period = time / 25.0f;
			spawnerModel.maxCount = countTarget % 3;
			AddTargetSpawner(FPoint(900.0f, 400.0f), duckPrefab, spawnerModel);
		}
		// else{ skip }
	}

	void StageScene::AddGun()
	{
		//// Орудие
		auto towerTexture = Core::resourceManager.Get<Render::Texture>("gun_tower");
		auto tower75Texture = Core::resourceManager.Get<Render::Texture>("gun_tower_75");
		auto tower50Texture = Core::resourceManager.Get<Render::Texture>("gun_tower_50");
		auto tower25Texture = Core::resourceManager.Get<Render::Texture>("gun_tower_25");
		auto barrelTexture = Core::resourceManager.Get<Render::Texture>("gun_barrel");
		auto redLampTexture = Core::resourceManager.Get<Render::Texture>("red_lamp");
		auto greenLampTexture = Core::resourceManager.Get<Render::Texture>("green_lamp");

		auto towerMaterial = new Material(towerTexture);
		auto tower75Material = new Material(tower75Texture);
		auto tower50Material = new Material(tower50Texture);
		auto tower25Material = new Material(tower25Texture);
		auto barrelMaterial = new Material(barrelTexture);
		auto redLampMaterial = new Material(redLampTexture);
		auto greenLampMaterial = new Material(greenLampTexture);

		Gun::Ptr gun = new Gun(Names::Gun, towerMaterial);
		gun->GetView()->SetOrigin(0.5f, 0.0f);
		gun->AddCollider(FPoint(0.0f, 0.0f), std::make_unique<CircleCollider>(Names::Head, 85.0f));

		View::Ptr tower75 = new View(Names::Tower75, tower75Material);
		gun->AddDecor(FPoint(0.0f, 0.5f), tower75.get());

		View::Ptr tower50 = new View(Names::Tower50, tower50Material);
		gun->AddDecor(FPoint(0.0f, 0.5f), tower50.get());

		View::Ptr tower25 = new View(Names::Tower25, tower25Material);
		gun->AddDecor(FPoint(0.0f, 0.5f), tower25.get());


		View::Ptr barrel = new View(Names::Barrel, barrelMaterial);
		barrel->SetOrigin(0.5f, 0.0);
		barrel->SetPivotOffset(0.0f, -1.6f);
		barrel->SetEulerAngle(-45.0f);
		gun->AddDecor(FPoint(0.0f, 0.0f), barrel.get());

		View::Ptr redLamp = new View(Names::RedLamp, redLampMaterial);
		View::Ptr greenLamp = new View(Names::GreenLamp, greenLampMaterial);
		gun->AddDecor(FPoint(0.0f, 0.5f), redLamp.get());
		gun->AddDecor(FPoint(0.0f, 0.5f), greenLamp.get());

		
		Gun::Model model;
		model.hp = 100;
		model.cooldown = 0.6f;
		// Интересно, а набор переменных понимает, что при попытке взять
		// дробное число нужно преобразовать его с плавающей точкой
		model.bulletSpeed = _settings.getInt(Names::BulletSpeedParameter);

		gun->SetModel(model);
		gun->SetPosition(FPoint(getWidth() * 0.5f, 0.0f));
		gun->SetBounds(gun->GetView()->Width(), gun->GetView()->Height() * 2);

		AddObject(gun);
	}

	void StageScene::AddWalls()
	{
		// Стены-невидимки
		float wallSize = 30.0f;
		Object::Ptr wallLeft = new Object(Names::Wall);
		wallLeft->SetBounds(FRect(0.0f, wallSize, 0.0f, height));
		wallLeft->SetPosition(-wallSize * 0.3f, height * 0.5f);

		Object::Ptr wallRight = new Object(*wallLeft);
		wallRight->SetPosition(width + wallSize * 0.3f, height * 0.5f);

		Object::Ptr wallTop = new Object(Names::Wall);
		wallTop->SetBounds(FRect(0.0f, width - 0.6f * wallSize, 0.0f, wallSize));
		wallTop->SetPosition(width * 0.5f, height + wallSize * 0.3f);

		Object::Ptr wallBottom = new Object(*wallTop);
		wallBottom->SetPosition(width * 0.5f, -wallSize * 0.3f);

		AddObject(wallLeft);
		AddObject(wallRight);
		AddObject(wallTop);
		AddObject(wallBottom);
	}

	void StageScene::AddTimeCountdown()
	{
		StageTimeCountdown::Ptr timer = new StageTimeCountdown(Names::Countdown, "starcraft");
		StageTimeCountdown::Model model;
		model.countdown = _settings.getInt(Names::TimeParameter);
		model.warningTime = 5.0f;
		model.scaleFactor.addKey(0.0f, 0.993f);
		model.scaleFactor.addKey(0.2f, 1.013f);
		model.scaleFactor.addKey(0.4f, 0.993f);
		model.scaleFactor.addKey(0.6f, 1.013f);
		model.scaleFactor.addKey(0.8f, 0.993f);
		model.scaleFactor.addKey(1.0f, 1.013f);
		timer->SetModel(model);
		timer->SetPosition(width, height);
		AddObject(timer);
	}
	
	void StageScene::AddMatchPoints()
	{
		MatchPoints::Ptr matchPoints = new MatchPoints(Names::Countdown, "starcraft");
		
		_matchPoints = matchPoints;

		MatchPoints::Model model;
		model.typePointsMap["damaged"] = 1;
		model.typePointsMap["destroyed"] = 30;
		model.typePointsMap["armor_damaged"] = 4;
		model.typePointsMap["armor_destroyed"] = 15;
		model.typePointsMap["headshot"] = 60;
		model.typePointsMap["missed"] = -10;
		model.typePointsMap["bomb_damaged"] = 10;
		model.typePointsMap["ground_hit"] = -10;
		model.typePointsMap["gun_damaged"] = -60;

		// FIXME: Try find impulse data or just take out-in ease
		model.scaleFactor.addKey(0.0f, 0.993f);
		model.scaleFactor.addKey(0.2f, 1.013f);
		model.scaleFactor.addKey(0.4f, 0.993f);
		model.scaleFactor.addKey(0.6f, 1.013f);
		model.scaleFactor.addKey(0.8f, 0.993f);
		model.scaleFactor.addKey(1.0f, 1.013f);
		matchPoints->SetModel(model);
		matchPoints->SetPosition(width, height);
		AddObject(matchPoints);
	}

	void StageScene::LoadSettings()
	{
		// FIXME: Файл у нас UTF-8, но имена параметров должны задаваться в латинице,
		// чтобы при работе с стандратной строкой не возникало сложностей
		// FIXME: При большом количестве разных параметров лучше перейти к xml формату
		// либо сделать для настроек соответствующую карту функторов для соответствующих типов
		std::regex intDefinitionRegex("([a-z]+)\\s*=\\s*(\\d+)", std::regex_constants::icase);
		std::regex stringDefinitionRegex("([a-z]+)\\s*=\\s*(.+)", std::regex_constants::icase);
		std::smatch definitionMatch;

		auto& stream = Core::fileSystem.OpenRead(Names::StageSettingsFile);
		IO::TextReader reader(stream.get());
		std::string definition;
		while (reader.ReadUtf8Line(definition)) {
			if (std::regex_match(definition, definitionMatch, intDefinitionRegex)) {
				int value = std::stoi(definitionMatch[2].str());
				std::string name = definitionMatch[1].str();
				std::transform(name.begin(), name.end(), name.begin(), ::tolower);
				_settings.setInt(name, value);

			}
			else if (std::regex_match(definition, definitionMatch, stringDefinitionRegex)) {
				std::string value = definitionMatch[2].str();
				std::string name = definitionMatch[1].str();
				std::transform(name.begin(), name.end(), name.begin(), ::tolower);
				_settings.setString(name, value);
			}
			// else { skip }
		}

	}

	void StageScene::Restart()
	{
		MM::manager.StopAll();
		Clear();
		_restartButton->setVisible(false);

		_countTarget = _settings.getInt(Names::CountTargetParameter);
		_showRestartButton = false;
		_tShowRestartButton = -1;

		AddWalls();
		AddGun();
		AddDuckSpawners();
		AddTimeCountdown();
		AddMatchPoints();

		_battleTrackId = MM::manager.PlayTrack("battle", true, 0.5f);

		StageInfoText::Ptr info = new StageInfoText("infotext", "bellflower");
		info->SetScale(0.4f, 0.4f);
		StageInfoText::Model infoModel;
		infoModel.text = "Music: Rompasso - Ignis";
		infoModel.startPoint = FPoint(width * -0.5f, height * 0.1f);
		infoModel.endPoint = FPoint(width * 0.5f, height * 0.1f);
		infoModel.time = 0.5f;
		infoModel.showTime = 30.0f;
		info->SetModel(infoModel);
		AddObjectLater(info);
	}

}