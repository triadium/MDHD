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
#include "Game/AnchovyTarget.h"

#include <Game/Scene.h>

namespace Game {

	AnchovyTarget::AnchovyTarget(Material * base)
		: Target(Names::Anchovy, Names::Target, base)
		, _t(0.0f)
	{
		_scaleFactor.addKey(0.0f, 0.0f);
		_scaleFactor.addKey(0.25f, 0.3f);
		_scaleFactor.addKey(0.50f, 0.5f);
		_scaleFactor.addKey(0.75f, 0.9f);
		_scaleFactor.addKey(1.0f, 1.0f);
		_scaleFactor.addKey(100.0f, 1.0f);
	}

	AnchovyTarget::AnchovyTarget(const AnchovyTarget & obj)
		: Target(obj)
	{
		Copy(obj);
	}

	AnchovyTarget & AnchovyTarget::operator=(const AnchovyTarget & obj)
	{
		if (this != &obj) {
			Target::Copy(obj);
			Copy(obj);
		}
		// else { noop }		

		return *this;
	}

	AnchovyTarget::~AnchovyTarget()
	{
	}

	void AnchovyTarget::Update(float dt)
	{
		_t += dt;
		if (_t >= 100.0f) {
			_t -= 100.0f;
		}
		// else { noop }

		float sf = _scaleFactor.getGlobalFrame(_t);
		auto radiance = GetDecor(Names::AnchovyRadiance);
		radiance->SetScale(sf, sf);

		Target::Update(dt);		
	}

	bool AnchovyTarget::OnEnterBoundsIntersect(Object * other)
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

	void AnchovyTarget::OnEnterCollision(Collider * collider, Object * other, Collider * otherCollider)
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

	void AnchovyTarget::Copy(const AnchovyTarget & obj)
	{
		_t = obj._t;
		_scaleFactor = obj._scaleFactor;
		//InternalUpdateData();
	}
}
