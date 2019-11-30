// Copyright (C) 2019 Aleksey Khachaturov
// https://github.com/triadium/MDHD
// License: GNU GPL 3 or later, see file LICENSE

#pragma once

namespace Game {

	class BoxCollider: public Collider {
	public:
		typedef std::array < math::Vector3, 4> Vertexes;

		BoxCollider(const std::string& name, const FPoint& baseSize);

		bool CollidesWith(Collider const & other) const override;
		void Update(const FPoint& position) override;
		Collider* Clone() override;

		const Vertexes& GetUpdatedVertexes() const;
		const math::Vector3& GetUpdatedCenter() const;

		// P - a point to be checked
		// A, B, C, D - vertexes of the box
		bool PointInRectangle(const math::Vector3& point, const Vertexes & vertexes) const;
	
#ifdef _DEBUG
		void Draw(const FPoint& position) override;
#endif // _DEBUG

	private:
		bool CollidesWith(const BoxCollider& box) const override;
		bool CollidesWith(const CircleCollider& circle) const override;

	private:
		Vertexes _vertexes;
		math::Vector3 _center;
		Vertexes _updatedVertexes;
		math::Vector3 _updatedCenter;
	};
}
