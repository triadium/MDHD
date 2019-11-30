// Copyright (C) 2019 Aleksey Khachaturov
// https://github.com/triadium/MDHD
// License: GNU GPL 3 or later, see file LICENSE

#pragma once

namespace Game {

	// ��������� ���������� ��������� �� ������������ ��� ��� � ������ �����������.
	// ��� ��� ������ ������������ ���������� ����� ����������� � ����� ������������ ���������
	// ������� �� ����� ����������� �� ������������ �����������, �� ���������� ���� �����
	// ������� ���������������.
	// ��� ������� ���������� ���������� ������������, ������ ��������� ���������, ������� ������� � 
	// �������� ����� �������� � ��������������� �������� ��� ������� ������� ����� ��������.

	class Collider {
	public:
		typedef std::unique_ptr<Collider> Unique;
		typedef std::vector<Unique> Container;

		// ���� �����������
		friend class BoxCollider;
		friend class CircleCollider;

		Collider(const std::string& name, const FPoint& baseSize);

		void* operator new(size_t i) { return _mm_malloc(i, 16); }
		void operator delete(void* p) { _mm_free(p); }

		const std::string& GetName() const;

		bool IsActive();
		void SetActive(bool value);

		const FPoint& GetOrigin() const;
		void SetOrigin(const FPoint& value);
		void SetOrigin(float x, float y) { SetOrigin(FPoint(x, y)); }

		const FPoint& GetPivotOffset() const;
		void SetPivotOffset(const FPoint& value);
		void SetPivotOffset(float x, float y) { SetPivotOffset(FPoint(x, y)); }

		float GetEulerAngle() const;
		void SetEulerAngle(float value);

		// ������� ������� ����� ������������� �������
		const FPoint& GetBaseSize() const;

		virtual bool CollidesWith(Collider const & other) const = 0;
		virtual void Update(const FPoint& position) = 0;
		void Update(float x, float y) { Update(FPoint(x, y)); }

		virtual Collider* Clone() = 0;

#ifdef _DEBUG
		virtual void Draw(const FPoint& position) = 0;
#endif // _DEBUG

	protected:
		// ���������� � ����� ������, � ��� ���������� ������� ��������� ��������������
		bool Collides(const BoxCollider& box, const CircleCollider& circle) const;

	private:
		void InternalUpdate();
		virtual bool CollidesWith(const BoxCollider& box) const = 0;
		virtual bool CollidesWith(const CircleCollider& circle) const = 0;

	protected:
		std::string _name;
		FPoint _baseSize;
		FPoint _origin;
		FPoint _pivotOffset;
		float _eulerAngle;
		FPoint _pivot;
		bool _isActive;
		math::Matrix4 _localTransform;
	};
}
