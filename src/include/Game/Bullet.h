// Copyright (C) 2019 Aleksey Khachaturov
// https://github.com/triadium/MDHD
// License: GNU GPL 3 or later, see file LICENSE

#pragma once

namespace Game {

	class Bullet: public Object
	{
	public:		
		typedef boost::intrusive_ptr<Bullet> Ptr;
		typedef std::vector<Ptr> Container;

		// ������ ������ �������
		struct Model {
			// ���� ����� �� ������� �������
			int power;
			// �������� (px/sec)
			float speed;
			// ���������� ���������, ������� ����� ��������� ������ ����� �������
			int ricochetCount;
			// ��������� �������� ��� ����� ������� ������������
			TimedSpline<float> speedFactor;
			// ���������� �� ��� � ��� ����� ������� ������������
			TimedSpline<float> deviationX;
			// ���������� �� ��� y ��� ����� ������� ������������
			TimedSpline<float> deviationY;
		};

		Bullet(const std::string& name, Material * base);
		explicit Bullet(const Bullet & obj);
		Bullet& operator=(const Bullet& obj);
		~Bullet() override;

		void Update(float dt) override;
		bool OnEnterBoundsIntersect(Object * other) override;
		void OnEnterCollision(Collider* collider, Object * other, Collider* otherCollider) override;

		const Model& GetModel() const;
		void SetModel(const Model& value);

		const FPoint& GetSourcePosition() const;
		void SetSourcePosition(const FPoint& value);
		void SetSourcePosition(float x, float y) { SetSourcePosition(FPoint(x, y)); }

		const FPoint& GetDirection() const;
		void SetDirection(const FPoint& value);

		void Fly(Scene * scene);

	private:
		void Copy(const Bullet& obj);
		void InternalUpdateData();
		void InternalUpdatePosition();
		void Explode();

	private:
		Model _model;
		FPoint _direction;
		FPoint _sourcePosition;
		float _st;
		float _dt;

		FPoint _across;
		FPoint _velocity;
		int _ricochetCountdown;
		ParticleEffectPtr _jetEffect;
	};

}
