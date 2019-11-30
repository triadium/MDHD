// Copyright (C) 2019 Aleksey Khachaturov
// https://github.com/triadium/MDHD
// License: GNU GPL 3 or later, see file LICENSE

#pragma once


namespace Game {

	class AnchovyTarget: public Target
	{
	public:		
		typedef boost::intrusive_ptr<AnchovyTarget> Ptr;
		typedef std::vector<Ptr> Container;
		
		AnchovyTarget(Material * base);
		explicit AnchovyTarget(const AnchovyTarget & obj);
		AnchovyTarget& operator=(const AnchovyTarget & obj);
		~AnchovyTarget() override;

		void Update(float dt) override;

		bool OnEnterBoundsIntersect(Object * other) override;
		void OnEnterCollision(Collider* collider, Object * other, Collider* otherCollider) override;

	protected:
		void Copy(const AnchovyTarget & obj);

	private:
		float _t;
		TimedSpline<float> _scaleFactor;
	};

}
