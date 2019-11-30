// Copyright (C) 2019 Aleksey Khachaturov
// https://github.com/triadium/MDHD
// License: GNU GPL 3 or later, see file LICENSE

#pragma once

namespace Game {

	class FishTarget: public Target
	{
	public:		
		typedef boost::intrusive_ptr<FishTarget> Ptr;
		typedef std::vector<Ptr> Container;
		
		FishTarget(Material * base);
		explicit FishTarget(const FishTarget & obj);
		FishTarget& operator=(const FishTarget & obj);
		~FishTarget() override;

		bool OnEnterBoundsIntersect(Object * other) override;
		void OnEnterCollision(Collider* collider, Object * other, Collider* otherCollider) override;

	protected:
		void Copy(const FishTarget & obj);
	};

}
