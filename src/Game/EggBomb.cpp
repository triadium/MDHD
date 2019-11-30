#include "stdafx.h"
#include "Game/Constants.h"
#include "Game/View.h"
#include "Game/Material.h"
#include "Game/Collider.h"
#include "Game/Object.h"
#include "Game/View.h"

#include "Game/Target.h"
#include "Game/FishTarget.h"
#include "Game/AnchovyTarget.h"
#include "Game/Gun.h"
#include "Game/EggBomb.h"

#include <Game/Scene.h>

namespace Game {

	EggBomb::EggBomb(const std::string & type, const std::string & name, Material * base)
		: Object(name, base)
		, _type(type)
		, _direction(FPoint(0.0f, -1.0f))
		// , _across(FPoint(0.0f, 1.0f))
		// , _t(0)
		, _gravity(10.0f)
		, _prefabIndex(0)
	{
	}

	EggBomb::EggBomb(const EggBomb & obj)
		: Object(obj)
	{
		Copy(obj);
	}

	EggBomb & EggBomb::operator=(const EggBomb & obj)
	{
		if (this != &obj) {
			Object::Copy(obj);
			Copy(obj);
		}
		// else { noop }		

		return *this;
	}

	EggBomb::~EggBomb()
	{
	}

	const std::string & EggBomb::GetType() const
	{
		return _type;
	}

	void EggBomb::Update(float dt)
	{
		// _t += dt;
		//if (_t >= 1.0f) {
		//	_t -= 1.0f;
		//}
		//// else { noop }

		//float sf = _model.speedFactor.getGlobalFrame(_t);
		_sourcePosition += (_direction * dt * dt * _gravity * _model.gravityCoefficient);

		InternalUpdatePosition();
	}

	bool EggBomb::OnEnterBoundsIntersect(Object * other)
	{
		auto& otherName = other->GetName();
		if (otherName == Names::Bullet) {
			return true;
		}
		else if (otherName == Names::Wall) {
			Message message(_name, "ground_hit", _model.power * 10);
			Core::messageManager.putMessage(message);
			Explode();
			TryCreateHatch();
			return false;
		}
		else if (otherName == Names::Gun) {
			return true;
		}
		else if (otherName == Names::Target) {
			return false;
		}
		return false;
	}

	void EggBomb::OnEnterCollision(Collider * collider, Object * other, Collider * otherCollider)
	{
		if (other->GetName() == Names::Bullet) {
			if (otherCollider->GetName() == Names::Bullet) {
				Message message(_name, "bomb_damaged");
				Core::messageManager.putMessage(message);
				Explode();
			}			
			// else { skip }
		}
		else if (other->GetName() == Names::Gun) {
			if (otherCollider->GetName() == Names::Head) {
				Explode();
				Gun * gun = reinterpret_cast<Gun*>(other);
				gun->TakeDamage(_model.power);
			}
			// else { skip }
		}		
		// else { skip }
	}

	const EggBomb::Model & EggBomb::GetModel() const
	{
		return _model;
	}

	void EggBomb::SetModel(const Model & value)
	{
		_model = value;
		InternalUpdateData();
	}

	const FPoint & EggBomb::GetSourcePosition() const
	{
		return _sourcePosition;
	}

	void EggBomb::SetSourcePosition(const FPoint & value)
	{
		_sourcePosition = value;
		InternalUpdatePosition();
	}

	void EggBomb::AddHatchPrefab(Target * prefab)
	{
		_prefabs.push_back(prefab);
	}

	void EggBomb::AddHatchPrefab(Target::Ptr & prefab)
	{
		_prefabs.push_back(prefab);
	}

	void EggBomb::Copy(const EggBomb & obj)
	{
		_type = obj._type;
		_model = obj._model;
		_sourcePosition = obj._sourcePosition;
		_direction = obj._direction;
		_gravity = obj._gravity;
		// _t = obj._t;
		_prefabs = obj._prefabs;
		_prefabIndex = obj._prefabIndex;
		InternalUpdateData();
	}

	void EggBomb::InternalUpdateData()
	{
	}

	void EggBomb::InternalUpdatePosition()
	{
		//float dx = _model.deviationX.getGlobalFrame(_t);
		//float dy = _model.deviationY.getGlobalFrame(_t);

		FPoint delta (0, 0); // dx * _direction + dy * _across;
		SetPosition(_sourcePosition + delta);
	}

	void EggBomb::Explode()
	{
		MM::manager.PlaySample("boom");
		_scene->AddEffect("Explosion", GetPosition());
		SetActive(false);
		_scene->RemoveObjectLater(this);
	}

	void EggBomb::TryCreateHatch()
	{
		int prefabCount = _prefabs.size();
		if (prefabCount > 0) {
			_prefabIndex = math::random(-1, prefabCount - 1);
		
			if (_prefabIndex >= 0) {
				auto target = CreateHatch();
				auto& position = GetPosition();
				if (position.x > _scene->getWidth() * 0.5f) {
					target->SetDirection(FPoint(-1, 0));
				}
				else {
					target->SetDirection(FPoint(1, 0));
				}
				target->SetSourcePosition(position + FPoint(0, 30));
				_scene->AddObjectLater(target);
			}
			// else { skip }
		}
		// else { noop }
	}

	Target::Ptr EggBomb::CreateHatch()
	{
		auto& prefab = _prefabs[_prefabIndex];
		if (prefab->GetType() == Names::Fish) {
			FishTarget::Ptr target = new FishTarget(*reinterpret_cast<FishTarget*>(prefab.get()));
			return target;
		}
		else if (prefab->GetType() == Names::Anchovy) {
			AnchovyTarget::Ptr target = new AnchovyTarget(*reinterpret_cast<AnchovyTarget*>(prefab.get()));
			return target;
		}
		else {
			Target::Ptr target = new Target(*prefab);
			return target;
		}
	}

}
