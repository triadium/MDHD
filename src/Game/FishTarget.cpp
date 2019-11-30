#include "stdafx.h"
#include "Game/Constants.h"
#include "Game/View.h"
#include "Game/Material.h"
#include "Game/Object.h"
#include "Game/View.h"
#include "Game/Collider.h"
#include "Game/Signal.h"
#include "Game/Gun.h"
#include "Game/Target.h"
#include "Game/FishTarget.h"

#include <Game/Scene.h>

namespace Game {

	FishTarget::FishTarget(Material * base)
		: Target(Names::Fish, Names::Target, base)
	{
	}

	FishTarget::FishTarget(const FishTarget & obj)
		: Target(obj)
	{
		Copy(obj);
	}

	FishTarget & FishTarget::operator=(const FishTarget & obj)
	{
		if (this != &obj) {
			Target::Copy(obj);
			Copy(obj);
		}
		// else { noop }		

		return *this;
	}

	FishTarget::~FishTarget()
	{
	}

	bool FishTarget::OnEnterBoundsIntersect(Object * other)
	{
		if (other->GetName() == Names::Wall) {
			this->FlipDirection();
			return false;
		}
		if (other->GetName() == Names::Bullet ||
			other->GetName() == Names::Gun) {
			return true;
		}
		// else { noop }
		return false;
	}

	void FishTarget::OnEnterCollision(Collider * collider, Object * other, Collider * otherCollider)
	{
		if (other->GetName() == Names::Gun) {
			if (otherCollider->GetName() == Names::Head) {
				Explode();
				Gun * gun = reinterpret_cast<Gun*>(other);
				gun->TakeDamage(GetModel().power);
			}
			// else { skip }
		}
		// else { skip }
	}

	void FishTarget::Copy(const FishTarget & obj)
	{
		//InternalUpdateData();
	}
}
