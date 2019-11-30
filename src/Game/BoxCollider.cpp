#include "stdafx.h"
#include "Game/Collider.h"
#include "Game/CircleCollider.h"
#include "Game/BoxCollider.h"

namespace Game {
	
	BoxCollider::BoxCollider(const std::string& name, const FPoint & baseSize)
		: Collider(name, baseSize)		
	{
		_vertexes[0] = math::Vector3(0.0f, 0.0f, 0.0f);
		_vertexes[1] = math::Vector3(_baseSize.x, 0.0f, 0.0f);
		_vertexes[2] = math::Vector3(_baseSize.x, _baseSize.y, 0.0f);
		_vertexes[3] = math::Vector3(0.0f, _baseSize.y, 0.0f);
		_center = math::Vector3(_baseSize.x * 0.5f, _baseSize.y * 0.5f, 0.0f);
	}

	bool BoxCollider::CollidesWith(Collider const & other) const
	{
		return other.CollidesWith(*this);
	}

	void BoxCollider::Update(const FPoint & position)
	{
		Render::device.PushMatrix();
		Render::device.MatrixTranslate(position);
		Render::device.MatrixMultiply(_localTransform);
		auto& transform = Render::device.GetMatrix(MatrixMode::ModelView);
		for (size_t i = 0; i < _updatedVertexes.size(); ++i) {
			_updatedVertexes[i] = _vertexes[i].TransformCoord(transform);
		}
		_updatedCenter = _center.TransformCoord(transform);
		Render::device.PopMatrix();
	}

	Collider * BoxCollider::Clone()
	{
		return new BoxCollider(*this);
	}

#ifdef _DEBUG
	void BoxCollider::Draw(const FPoint& position)
	{		
		// Предполагается, что внешний метод прорисовки 
		// деактивировал тестурирование, определил цвет
		// и прозрачность
		Render::device.PushMatrix();
		Render::device.MatrixTranslate(position);
		Render::device.MatrixMultiply(_localTransform);
		Render::DrawRect(IRect(0, 0, _baseSize.x, _baseSize.y));
		Render::device.PopMatrix();
	}
#endif // _DEBUG

	const BoxCollider::Vertexes & BoxCollider::GetUpdatedVertexes() const
	{
		return _updatedVertexes;
	}

	const math::Vector3 & BoxCollider::GetUpdatedCenter() const
	{
		return _updatedCenter;
	}

	// P - a point to be checked
	// A, B, C, D - vertexes of the box
	// 0 <= AP.AB <= AB.AB and 0 <= AP.AD <= AD.AD
	bool BoxCollider::PointInRectangle(const math::Vector3& point, const BoxCollider::Vertexes & vertexes) const
	{
		auto vAP = point - vertexes[0];
		auto vAB = vertexes[1] - vertexes[0];
		auto dotAPvAB = vAP.DotProduct(vAB);
		auto dotABvAB = vAB.DotProduct(vAB);

		if (0 <= dotAPvAB && dotAPvAB <= dotABvAB) {
			auto vAD = vertexes[3] - vertexes[0];
			auto dotAPvAD = vAP.DotProduct(vAD);
			auto dotADvAD = vAD.DotProduct(vAD);
			return (0 <= dotAPvAD && dotAPvAD <= dotADvAD);
		}
		// else { noop }

		return false;
	}

	bool BoxCollider::CollidesWith(const BoxCollider& box) const
	{
		auto& other = box.GetUpdatedVertexes();
		for (auto& vertex : _updatedVertexes) {
			if (PointInRectangle(vertex, other)) {
				return true;
			}
			// else { noop }
		}

		for (auto& vertex : other) {
			if (PointInRectangle(vertex, _updatedVertexes)) {
				return true;
			}
			// else { noop }
		}

		return false;
	}

	bool BoxCollider::CollidesWith(const CircleCollider& circle) const
	{
		return Collides(*this, circle);
	}
}