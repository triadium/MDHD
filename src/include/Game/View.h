// Copyright (C) 2019 Aleksey Khachaturov
// https://github.com/triadium/MDHD
// License: GNU GPL 3 or later, see file LICENSE

#pragma once

namespace Game {

	// ������������� �� ����� ������ ��� ���������� � ���������� �������� 
	// ���������� ��������� �������������. � ������� ������������� ���� 
	// ����� ��������, ������������ ��������������� �� ���� XY � ���� 
	// �������� � ��������� ������ (������ ��� Z).
	// ���������� ������������� ���������� � ������� ������������ ���������.
	// ����� �������� �������������� ��� ���������� �� ������ ��������� ���������.
	// ������ ��������� ��������� � ���������� �������� � ��������������� ��������, 
	// ������� ������������ �� ��������� ������ � ������  ���������.
	// ���������� ������� ��������� ���������� ��������� � ������ ����� ����,
	// �� �� ��������� ��� ���������� � ����� �������������.
	// ������ � ��������� �����, ����������, ���������, �������� �������������� �����
	// Get*/Set* �������, � �� ��������, ��� ��� ���� ������������� ���������
	// ���������� ��������� ������������� � �� ���������� �����������
	// �������� ������� ���������� �������.
	
	class View: public RefCounter
	{
	public:
		typedef boost::intrusive_ptr<View> Ptr;
		typedef std::vector<Ptr> Container;

		View(const std::string& name, Material * material);
		//View(const View& view);

		void* operator new(size_t i) { return _mm_malloc(i, 16); }
		void operator delete(void* p) { _mm_free(p); }

		const std::string& GetName() const;
		bool IsVisible();
		void SetVisible(bool value);

		void Draw(const FPoint& position);
		void Draw(float x, float y) { Draw(FPoint(x, y)); }
		void DrawIntermediate(const FPoint& position);
		void DrawIntermediate(float x, float y) { DrawIntermediate(FPoint(x, y)); }

		// TODO: ��������� � ��������� ����� ������������. 
		// ����� �������������� ��� ��� ������������� �� �����, ��� � ��� 
		// ������������� ���������� ��������� ������������ (�����������).
		// TODO: ��� ��� �������� �������� � ������ ������������� �����������, 
		// �� ����� �������� � �������� ��������� � ����� �������������,
		// � ��� ������� ������ ��������� ������������� �����.
		const FPoint& GetOrigin() const;
		void SetOrigin(const FPoint& value);
		void SetOrigin(float x, float y) { SetOrigin(FPoint(x, y)); }

		const FPoint& GetPivotOffset() const;
		void SetPivotOffset(const FPoint& value);
		void SetPivotOffset(float x, float y) { SetPivotOffset(FPoint(x, y)); }

		const FPoint& GetScale() const;
		void SetScale(const FPoint& value);
		void SetScale(float x, float y) { SetScale(FPoint(x, y)); }

		// ����������� ������������� -1 | +1
		const IPoint& GetFlips() const;
		// ����� ������� 0 ��� ���������, ����� �� ������ ����������� �� ����
		void SetFlips(int x, int y);
		void SetFlips(const IPoint& flips) { SetFlips(flips.x, flips.y); }
		// "������" ��� ��������� - ������� ����������� ��� ���������
		void Flip(bool x, bool y);

		float GetEulerAngle() const;
		void SetEulerAngle(float value);

		Material * GetMaterial() const;
		void SetMaterial(Material * value);

		int Width() const;
		int Height() const;

		FPoint GetPin(const FPoint& offset) const;

		const math::Matrix4& GetLocalTransform() const;

	private:
		void InternalUpdate();

	private:
		std::string _name;
		Material::Ptr _material;
		IPoint _flips;
		FPoint _origin;
		FPoint _pivotOffset;
		FPoint _scale;
		float _eulerAngle;
		FPoint _pivot;
		bool _isVisible;
		math::Matrix4 _localTransform;
	};
}
