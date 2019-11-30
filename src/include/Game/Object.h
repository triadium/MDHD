// Copyright (C) 2019 Aleksey Khachaturov
// https://github.com/triadium/MDHD
// License: GNU GPL 3 or later, see file LICENSE

#pragma once

namespace Game {

	class Scene;

	// ������ ����� �������� ������� ��� ��������� ���������� ��������������:
	// ������� ������, ������� ����������� ������������, ������������� �� ������
	// ��� ��������� ����� �������, ��� � ������� ������� ���� ���� �������
	// ������������� � ��������� ����� �������� �����������. ������ ������� ��������� 
	// ������� �������������, � ����� ��� �� ������� ��� ����������.
	// ��� ������������� ����� ����� ��������� ���������� �� ���� �������, ����� �������
	// ������������� ��������� ����������. ����� ������������� ���������� �����������
	// ��� ���������� �� ����� �������� �������� ������������� � ��������������� ��������.
	// ��������� ������������� �������� ������������� �������� ������������ ��� ���� �����������.
	// (��������������� ������ �������������� �� ��������� �������� ������ � ������ 
	// �������� �������������)
	// ������ ����� ��������� ����� ����������, �� ������� ������������ AABB 
	// � ������ ��������� ������������� ��� ������� ����������� ������������. 
	// ���� ���������� ��� � �������, �� ����� ������ �� ��������� � ���������� 
	// ����������� ������������ � �������� ������� �����.
	// �������� ���������� ������������ ������ ����� �������� �������� �������������
	// ���������� ��� ������� �������� � �������� ������, � �� � ��������������� ��� 
	// ��� ����� �������� � ���������� � �������������.
	// ������ ����� ���� �������� � �����, � ����� ���� ������ �� �����.
	// ������ ����� ����� ���������� � ����� ���������� ����� �� ������������ ���.
	// ��� ������������� ���������/����������� ����������� ������� �������� �����
	// � �� ���������� ����������������� ��������.
	// � ���������� ���������� �������� ������ �� ������ ��� ����������, ��� ���
	// ���������������� ������ � ������� ���������� ����� ���������� � �����.

	class Object: public RefCounter
	{
	public:
		typedef boost::intrusive_ptr<Object> Ptr;
		typedef std::vector<Ptr> Container;

		// ������ ������. ����� ���������� ������� ��� ������� � ������� ������������
		Object(const std::string& name);
		Object(const std::string& name, View * base);
		Object(const std::string& name, Material * base);
		
		explicit Object(const Object & obj);
		Object& operator=(const Object& obj);
		~Object() override;

		const std::string& GetName() const;

		bool IsActive();
		void SetActive(bool value);		

		Scene * GetScene() const;
		void SetScene(Scene * value);

		virtual void Draw();

		View * GetView() const;
		void SetView(View * base);

		const View::Container& GetDecors() const;
		void AddDecor(const FPoint& pinOffset, View * decor);
		void AddDecor(const FPoint& pinOffset, View * decor, size_t index);
		View * AddDecor(const FPoint& pinOffset, Material * decor);
		View * AddDecor(const FPoint& pinOffset, Material * decor, size_t index);
		void RemoveDecor(View * decor);
		View * GetDecor(const std::string& name) const;
		const FPoint& GetDecorPin(size_t index) const;

		const FPoint& GetPosition() const;
		void SetPosition(const FPoint& value);
		void SetPosition(float x, float y) { SetPosition(FPoint(x, y)); }

		float GetEulerAngle() const;
		void SetEulerAngle(const float value);

		const FPoint& GetScale() const;
		void SetScale(const FPoint& value);
		void SetScale(float x, float y) { SetScale(FPoint(x, y)); }

		// ����������� ������� -1 | +1
		const IPoint& GetFlips() const;
		// ���������� ����������� ����� ������� ���������� �� 
		// ����������� �������� ���� � �����������
		// ����� ������� 0 ��� ���������, ����� �� ������ ����������� �� ����
		void SetFlips(int x, int y);
		void SetFlips(const IPoint& flips) { SetFlips(flips.x, flips.y); }
		// "������" ��� ��������� - ������� ����������� ��� ���������
		void Flip(bool x, bool y);

		const FRect& GetBounds() const;
		void SetBounds(const FRect& value);
		void SetBounds(const FPoint& value) { SetBounds(FRect(0.0f, value.x, 0.0f, value.y)); }
		void SetBounds(float width, float height) { SetBounds(FRect(0.0f, width, 0.0f, height)); }
		const FRect& GetAabb() const;
		const Collider::Container& GetColliders() const;
		void AddCollider(const FPoint& pinOffset, Collider::Unique&& collider);
		// void RemoveCollider(Collider * collider);
		void UpdateColliders();

		// ���� �� ����� ��������� ���������� ��� ������ ���� ��������,
		// �� ������� false
		virtual bool OnEnterBoundsIntersect(Object * other);
		virtual void OnEnterCollision(Collider* collider, Object * other, Collider* otherCollider);

		virtual void Update(float dt);

		virtual void MouseDown(const IPoint &mouse_pos);
		virtual void MouseUp(const IPoint& mouse_pos);

#ifdef _DEBUG
		void DrawBoundsAndColliders();
#endif // _DEBUG

	protected:
		FPoint GetPin(const FPoint & offset) const;
		void Copy(const Object& obj);

	protected:
		Scene * _scene;
		View::Ptr _view;
		std::string _name;

	private:
		void InternalUpdateBounds();
		void InternalUpdateAabb();

	private:
		bool _isActive;
		FPoint _position;
		FPoint _scale;
		IPoint _flips;
		View::Container _decors;
		std::vector<FPoint> _decorPins;
		// ������� � ��������� �������
		FRect _bounds;
		// �������, ��������� �� �������
		FRect _aabb;
		Collider::Container _colliders;
		std::vector<FPoint> _colliderPins;
	};

}
