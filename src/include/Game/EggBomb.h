// Copyright (C) 2019 Aleksey Khachaturov
// https://github.com/triadium/MDHD
// License: GNU GPL 3 or later, see file LICENSE

#pragma once

namespace Game {

	class EggBomb: public Object
	{
	public:		
		typedef boost::intrusive_ptr<EggBomb> Ptr;
		typedef std::vector<Ptr> Container;
		
		// ������ ������ �����
		struct Model {
			// ���� ����� �� ������ �����
			int power;
			// ����������� ���������
			float gravityCoefficient;
		};

		EggBomb(const std::string & type, const std::string& name, Material * base);
		explicit EggBomb(const EggBomb & obj);
		EggBomb& operator=(const EggBomb& obj);
		~EggBomb() override;

		const std::string& GetType() const;

		void Update(float dt) override;
		bool OnEnterBoundsIntersect(Object * other) override;
		void OnEnterCollision(Collider* collider, Object * other, Collider* otherCollider) override;

		const Model& GetModel() const;
		void SetModel(const Model& value);

		const FPoint& GetSourcePosition() const;
		void SetSourcePosition(const FPoint& value);
		void SetSourcePosition(float x, float y) { SetSourcePosition(FPoint(x, y)); }

		void AddHatchPrefab(Target * prefab);
		void AddHatchPrefab(Target::Ptr & prefab);


	protected:
		void Copy(const EggBomb& obj);

	private:
		void InternalUpdateData();
		void InternalUpdatePosition();
		void Explode();
		void TryCreateHatch();
		Target::Ptr CreateHatch();

	private:
		// ��� ����
		// FIXME: ������� ������ ������ ������ ��� ������
		// ����� ������� � ����� �������� � ���� ����� ECS, 
		// �� ���� ������� ������ �� ���������� � �����������
		std::string _type;

		// ����������� ��������
		Model _model;
		FPoint _direction;
		FPoint _sourcePosition;
		// float _t;
		float _gravity;

		Target::Container _prefabs;
		int _prefabIndex;

		// ����������� ��������
		// FPoint _across;
		// FPoint _velocity;
	};

}
