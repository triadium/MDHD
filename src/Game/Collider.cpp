#include "stdafx.h"
#include "Game/BoxCollider.h"
#include "Game/CircleCollider.h"
#include "Game/Collider.h"

namespace Game {

	Collider::Collider(const std::string& name, const FPoint& baseSize)
		: _name(name)
		, _eulerAngle(0)
		, _isActive(true)
		, _baseSize(baseSize)
		, _origin(0.5f, 0.5f)
		, _pivotOffset(0.0f, 0.0f)
	{
		InternalUpdate();
	}

	const std::string & Collider::GetName() const
	{
		return _name;
	}

	bool Collider::IsActive()
	{
		return _isActive;
	}

	void Collider::SetActive(bool value)
	{
		_isActive = value;
		InternalUpdate();
	}

	const FPoint & Collider::GetOrigin() const
	{
		return _origin;
	}

	void Collider::SetOrigin(const FPoint & value)
	{
		_origin = value;
		InternalUpdate();
	}

	const FPoint & Collider::GetPivotOffset() const
	{
		return _pivotOffset;
	}

	void Collider::SetPivotOffset(const FPoint & value)
	{
		_pivotOffset = value;
		InternalUpdate();
	}

	float Collider::GetEulerAngle() const
	{
		return _eulerAngle;
	}

	void Collider::SetEulerAngle(float value)
	{
		_eulerAngle = value;
		InternalUpdate();
	}

	const FPoint & Collider::GetBaseSize() const
	{
		return _baseSize;
	}

	bool Collider::Collides(const BoxCollider & box, const CircleCollider & circle) const
	{
		auto& boxCenter = box.GetUpdatedCenter();
		auto& circleCenter = circle.GetUpdatedCenter();
		float dx = math::abs(circleCenter.x - boxCenter.x);
		float dy = math::abs(circleCenter.y - boxCenter.y);
		float radius = circle.GetRadius();
		auto& boxSize = box.GetBaseSize();
		float halfWidth = boxSize.x * 0.5f;
		float halfHeight = boxSize.y * 0.5f;

		if (dx > (halfWidth + radius)) { return false; }
		if (dy > (halfHeight + radius)) { return false; }

		if (dx <= halfWidth) { return true; }
		if (dy <= halfHeight) { return true; }

		float cornerDistance = (dx - halfWidth) * (dx - halfWidth) + (dy - halfHeight) * (dy - halfHeight);

		return (cornerDistance <= (radius * radius));
	}

	void Collider::InternalUpdate()
	{
		_pivot = -1 * (_origin + _pivotOffset);
		if (_isActive) {
			// Масштаб применяется родительской трансформации, поэтому
			// только вращение и перемещение
			Render::device.PushMatrix();
			Render::device.ResetMatrix();
			Render::device.MatrixRotate(math::Vector3::UnitZ, _eulerAngle);
			Render::device.MatrixTranslate(_pivot.x * _baseSize.x, _pivot.y * _baseSize.y, 0);
			_localTransform = Render::device.GetMatrix(MatrixMode::ModelView);
			Render::device.PopMatrix();
		}
		else {
			_localTransform = math::Matrix4::Identity;
		}
	}

}