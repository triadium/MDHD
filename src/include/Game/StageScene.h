// Copyright (C) 2019 Aleksey Khachaturov
// https://github.com/triadium/MDHD
// License: GNU GPL 3 or later, see file LICENSE

#pragma once

namespace Game {
	
	// ����� ������
	class StageScene : public Scene
	{
	public:

		StageScene(const std::string& name, rapidxml::xml_node<>* elem);

		void AcceptMessage(const Message& message) override;

		void Update(float dt) override;

#ifdef _DEBUG
		void Draw() override;
		void KeyPressed(int keyCode) override;
#endif // _DEBUG

	private:
		void Init();
		// ��������� ������ ������ ���������� "�����" ���������
		Target::Ptr CreateDuckPrefab(EggBomb::Ptr& eggBombPrefab, const Material::Ptr& baseMaterial, const Material::Ptr& wingMaterial);
		Target::Ptr CreateArmoredDuckPrefab(EggBomb::Ptr& eggBombPrefab, const Material::Ptr& baseMaterial, const Material::Ptr& wingMaterial, const Material::Ptr& armorMaterial);
		FishTarget::Ptr CreateFishPrefab(const Material::Ptr& baseMaterial);
		AnchovyTarget::Ptr CreateAnchovyPrefab(const Material::Ptr& baseMaterial, const Material::Ptr& radianceMaterial);
		EggBomb::Ptr CreateEggBombPrefab(const Material::Ptr& baseMaterial);
		void AddTargetSpawner(const FPoint& position, const Target::Ptr& prefab, const TargetSpawner::Model& model);
		void AddDuckSpawners();
		void AddGun();
		void AddWalls();
		void AddTimeCountdown();
		void AddMatchPoints();
		// ��������� ��������� ����, ���� ��� ��� �� ���� ���������
		void LoadSettings();
		// �������� ������� ������
		void Restart();
	private:		
		// ������������ ��������� ���������
		//����� �� ����������� ������ (�������)
		//int Time
		//���������� ���������� ����� ��� ����������� ������
		//int CountTarget
		//�������� ������� (px/sec)
		//int BulletSpeed
		VariableSet _settings;
		bool _settingsLoaded;

		float _time;
		int _countTarget;
		int _battleTrackId;
		// TODO: ���� ����������� � ��������
		float _tShowRestartButton;
		bool _showRestartButton;
		GUI::WidgetPtr _restartButton;
		Object::Ptr _matchPoints;
#ifdef _DEBUG
		float _dtFactor;
#endif // _DEBUG

	};

}
