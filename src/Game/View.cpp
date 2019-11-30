#include "stdafx.h"
#include "Game/Material.h"
#include "Game/View.h"

namespace Game {

	View::View(const std::string& name, Material * material)
		: _name(name)
		, _material(material)
		, _flips(1, 1)
		, _isVisible(true)
		, _origin(0.5f, 0.5f)
		, _pivotOffset(0.0f, 0.0f)
		, _scale(1.0f, 1.0f)
		, _eulerAngle(0.0f)
		, _localTransform(math::Matrix4::Identity)
	{
		InternalUpdate();
	}

	//View::View(const View & view)
	//	: RefCounter(view)
	//{		
	//	_name = view._name;
	//	_material = view._material;
	//	_origin = view._origin;
	//	_pivotOffset = view._pivotOffset;
	//	_scale = view._scale;
	//	_eulerAngle = view._eulerAngle;
	//	_pivot = view._pivot;
	//	_localTransform = view._localTransform;
	//	_isVisible = view._isVisible;
	//}

	const std::string & View::GetName() const
	{
		return _name;
	}

	bool View::IsVisible()
	{
		return _isVisible;
	}

	void View::SetVisible(bool value)
	{
		_isVisible = value;
		InternalUpdate();
	}

	void View::Draw(const FPoint& position)
	{
		if (_material && _isVisible) {
			Render::device.PushMatrix();
			DrawIntermediate(position);
			Render::device.PopMatrix();
		}
		// else { noop }
	}

	void View::DrawIntermediate(const FPoint& position)
	{
		if (_material && _isVisible) {
			// FIXME: –одительска€ матрица уже должна быть загружена.
			// —начала осуществим смещение позиции, а затем применим
			// локальную матрицу к текущей и отобразим представление
			Render::device.MatrixTranslate(position);
			Render::device.MatrixMultiply(_localTransform);

			// ¬ данном случае идет прив€зка и отв€зка текстур материала
			// шейдера и восстановление режимов смешивани€.
			_material->Bind();
			Render::DrawQuad(0, 0, _material->Width(), _material->Height());
			_material->Unbind();
		}
		// else { noop }
	}

	const FPoint & View::GetOrigin() const
	{
		return _origin;
	}

	void View::SetOrigin(const FPoint & value)
	{
		_origin = value;
		InternalUpdate();
	}

	const FPoint & View::GetPivotOffset() const
	{
		return _pivotOffset;
	}

	void View::SetPivotOffset(const FPoint & value)
	{
		_pivotOffset = value;
		InternalUpdate();
	}

	const FPoint & View::GetScale() const
	{
		return _scale;
	}

	void View::SetScale(const FPoint & value)
	{
		_scale = value;
		InternalUpdate();
	}

	const IPoint & View::GetFlips() const
	{
		return _flips;
	}

	void View::SetFlips(int x, int y)
	{
		_flips.x = x == 0 ? _flips.x : (x > 0 ? 1 : -1);
		_flips.y = y == 0 ? _flips.y : (y > 0 ? 1 : -1);
	}

	void View::Flip(bool x, bool y)
	{
		_flips.x *= (x ? -1 : 1);
		_flips.y *= (y ? -1 : 1);
		InternalUpdate();
	}

	float View::GetEulerAngle() const
	{
		return _eulerAngle;
	}

	void View::SetEulerAngle(float value)
	{
		_eulerAngle = value;
		InternalUpdate();
	}

	Material * View::GetMaterial() const
	{
		return _material.get();
	}

	void View::SetMaterial(Material * value)
	{
		_material = value;
		InternalUpdate();
	}

	int View::Width() const
	{
		return _material ? _material->Width() : 0;
	}

	int View::Height() const
	{
		return _material ? _material->Height() : 0;
	}

	FPoint View::GetPin(const FPoint & offset) const
	{
		FPoint pin = _origin + offset;
		pin.Scale(Width(), Height());
		return pin;
	}

	const math::Matrix4 & View::GetLocalTransform() const
	{
		return _localTransform;
	}

	void View::InternalUpdate() {
		_pivot = -1 * (_origin + _pivotOffset);
		if (_material && _isVisible) {

			int width = _material->Width();
			int height = _material->Height();
	
			// FIXME: ћне кажетс€, что расчет локальной матрицы представлени€ 
			// лучше делать все таки на CPU, а потом загружать конечный результат дл€ 
			// мирового преобразовани€ в GPU, так как расчет и получение результата
			// идет последовательно, но только в случае поддержки платформой SIMD команд.

			// “ак как операции не коммутативны, то последовательность всегда така€
			// a) ¬ращение
			// b) ћасштабирование, возможно даже разное по разным ос€м
			// c) ѕеремещение в точку вращени€ с учетом размера изображени€
			Render::device.PushMatrix();
			Render::device.ResetMatrix();
			Render::device.MatrixRotate(math::Vector3::UnitZ, _eulerAngle);
			Render::device.MatrixScale(_flips.x * _scale.x, _flips.y * _scale.y, 1.0f);
			Render::device.MatrixTranslate(_pivot.x * width, _pivot.y * height, 0.0f);
			_localTransform = Render::device.GetMatrix(MatrixMode::ModelView);
			Render::device.PopMatrix();			
		}
		else {
			_localTransform = math::Matrix4::Identity;
		}
	}	
}