// Copyright (C) 2019 Aleksey Khachaturov
// https://github.com/triadium/MDHD
// License: GNU GPL 3 or later, see file LICENSE

#pragma once

namespace Game {

	class DuckTarget: public Target
	{
	public:		
		typedef boost::intrusive_ptr<DuckTarget> Ptr;
		typedef std::vector<Ptr> Container;
		
		DuckTarget(EggBomb * eggBombPrefab, Material * base);
		explicit DuckTarget(const DuckTarget & obj);
		DuckTarget& operator=(const DuckTarget& obj);
		~DuckTarget() override;

		bool OnEnterBoundsIntersect(Object * other) override;
		void TakeDamage(int value) override;
	protected:
		void Copy(const DuckTarget& obj);

	private:
		EggBomb::Ptr _eggBombPrefab;
	};

}
