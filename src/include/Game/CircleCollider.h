// Copyright (C) 2019 Aleksey Khachaturov
// https://github.com/triadium/MDHD
// License: GNU GPL 3 or later, see file LICENSE

#pragma once

namespace Game {

	class CircleCollider: public Collider {
	public:

		CircleCollider(const std::string& name, float radius);

		bool CollidesWith(Collider const & other) const override;
		void Update(const FPoint& position) override;
		Collider* Clone() override;

		float GetRadius() const;
		const math::Vector3& GetUpdatedCenter() const;

#ifdef _DEBUG
		void Draw(const FPoint& position) override;
#endif // _DEBUG

	private:
		bool CollidesWith(const BoxCollider& box) const override;
		bool CollidesWith(const CircleCollider& circle) const override;

	private:
		float _radius;
		math::Vector3 _center;
		math::Vector3 _updatedCenter;
	};
}
