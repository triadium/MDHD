#include "stdafx.h"
#include "Game/Collider.h"
#include "Game/BoxCollider.h"
#include "Game/CircleCollider.h"

namespace Game {

	CircleCollider::CircleCollider(const std::string& name, float radius)
		: Collider(name, FPoint(2 * radius, 2 * radius))
		, _radius(radius)
	{
		_center = math::Vector3(radius, radius, 0.0f);
	}

	bool CircleCollider::CollidesWith(Collider const & other) const
	{
		return other.CollidesWith(*this);
	}

	void CircleCollider::Update(const FPoint & position)
	{
		Render::device.PushMatrix();
		Render::device.MatrixTranslate(position);
		Render::device.MatrixMultiply(_localTransform);
		auto& transform = Render::device.GetMatrix(MatrixMode::ModelView);
		_updatedCenter = _center.TransformCoord(transform);
		Render::device.PopMatrix();
	}

	Collider * CircleCollider::Clone()
	{
		return new CircleCollider(*this);
	}

#ifdef _DEBUG
	void CircleCollider::Draw(const FPoint& position)
	{
		// Предполагается, что внешний метод прорисовки 
		// деактивировал тестурирование, определил цвет
		// и прозрачность
		Render::device.PushMatrix();
		Render::device.MatrixTranslate(position.x, position.y, 0);
		Render::device.MatrixMultiply(_localTransform);

		Render::DrawRect(IRect(0, 0, _baseSize.x, _baseSize.y));

		Render::device.PopMatrix();
	}
#endif // _DEBUG

	bool CircleCollider::CollidesWith(const BoxCollider& box) const
	{
		return Collides(box, *this);
	}

	bool CircleCollider::CollidesWith(const CircleCollider& circle) const
	{
		auto distance = _updatedCenter - circle._updatedCenter;		
		// TODO: Перенести значение квадрата радиуса в поле
		return distance.LengthSq() <= (_radius * _radius + circle._radius * circle._radius);
	}

	float CircleCollider::GetRadius() const
	{
		return _radius;
	}

	const math::Vector3 & CircleCollider::GetUpdatedCenter() const
	{
		return _updatedCenter;
	}
}