// Copyright (C) 2019 Aleksey Khachaturov
// https://github.com/triadium/MDHD
// License: GNU GPL 3 or later, see file LICENSE

#pragma once

namespace Game {

	class Target: public Object
	{
	public:		
		typedef boost::intrusive_ptr<Target> Ptr;
		typedef std::vector<Ptr> Container;
		
		// FIXME: ����� ������� � ������� ������� ������ ������,
		// ������� ����� ������������ ����� VariableSet.
		// ��� �������� �������� ��� ������ (�� ����� ������ cast), 
		// �� ������ ������������������ � ������������.
		// ������ ������ ����
		struct Model {
			// ����, ������� ������� ����, ���� �������� � �����
			int power;
			// ��������
			int hp;
			// �����
			int ap;
			// �������� (px/sec)
			float speed;
			// ��������� �������� ��� ����� ������� ������������
			TimedSpline<float> speedFactor;
			// ���������� �� ��� � ��� ����� ������� ������������
			TimedSpline<float> deviationX;
			// ���������� �� ��� y ��� ����� ������� ������������
			TimedSpline<float> deviationY;
		};

		Target(const std::string & type, const std::string& name, Material * base);
		explicit Target(const Target & obj);
		Target& operator=(const Target& obj);
		~Target() override;

		const std::string& GetType() const;

		void Update(float dt) override;

		const Model& GetModel() const;
		void SetModel(const Model& value);

		const FPoint& GetSourcePosition() const;
		void SetSourcePosition(const FPoint& value);
		void SetSourcePosition(float x, float y) { SetSourcePosition(FPoint(x, y)); }

		const FPoint& GetDirection() const;
		void SetDirection(const FPoint& value);

		void FlipDirection();
		virtual void TakeDamage(int value);

	protected:
		void Copy(const Target& obj);

		void Explode();

	private:
		void InternalUpdateData();
		void InternalUpdatePosition();
		void TakeHpDamage(int value);
		void TakeApDamage(int value);

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
		float _t;

		// ����������� ��������
		bool _armored;
		FPoint _across;
		FPoint _velocity;
		View::Ptr _armorDecor;
	};

}
