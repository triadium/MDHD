#include "stdafx.h"
#include "Game/Constants.h"
#include "Game/View.h"
#include "Game/Material.h"
#include "Game/Object.h"
#include "Game/View.h"
#include "Game/Signal.h"
#include "Game/Target.h"

#include <Game/Scene.h>

namespace Game {

	Target::Target(const std::string & type, const std::string & name, Material * base)
		: Object(name, base)
		, _type(type)
		, _direction(FPoint(1.0f, 0.0f))
		, _across(FPoint(0.0f, 1.0f))
		, _t(0)
	{
	}

	Target::Target(const Target & obj)
		: Object(obj)
	{
		Copy(obj);
	}

	Target & Target::operator=(const Target & obj)
	{
		if (this != &obj) {
			Object::Copy(obj);
			Copy(obj);
		}
		// else { noop }		

		return *this;
	}

	Target::~Target()
	{
	}

	const std::string & Target::GetType() const
	{
		return _type;
	}

	void Target::Update(float dt)
	{
		_t += dt;
		if (_t >= 1.0f) {
			_t -= 1.0f;
		}
		// else { noop }

		float sf = _model.speedFactor.getGlobalFrame(_t);
		_sourcePosition += (_velocity * dt * sf);

		InternalUpdatePosition();
	}

	const Target::Model & Target::GetModel() const
	{
		return _model;
	}

	void Target::SetModel(const Model & value)
	{
		_model = value;
		InternalUpdateData();
	}

	const FPoint & Target::GetSourcePosition() const
	{
		return _sourcePosition;
	}

	void Target::SetSourcePosition(const FPoint & value)
	{
		_sourcePosition = value;
		InternalUpdatePosition();
	}

	const FPoint & Target::GetDirection() const
	{
		return _direction;
	}

	void Target::SetDirection(const FPoint & value)
	{
		int sx = math::sign(_direction.x);
		_direction = value.Normalized();

		// Меняем направление только по X 
		// и только если направление сменилось 
		Flip((sx * math::sign(_direction.x)) < 0, false);

		_across = _direction.Rotated(math::PI / 2);
		_velocity = _direction * _model.speed;
	}

	void Target::FlipDirection()
	{
		SetDirection(_direction * -1);
		_velocity = _direction * _model.speed;
	}

	void Target::TakeDamage(int value)
	{
		if (_armored) {
			TakeApDamage(value);
		}
		else {
			TakeHpDamage(value);
		}
	}

	void Target::Explode()
	{
		MM::manager.PlaySample("boom");
		_scene->AddEffect("Explosion", GetPosition());
		SetActive(false);
		_scene->RemoveObjectLater(this);
	}

	void Target::Copy(const Target & obj)
	{
		_type = obj._type;
		_model = obj._model;
		_sourcePosition = obj._sourcePosition;
		_direction = obj._direction;
		_t = obj._t;
		InternalUpdateData();
	}

	void Target::InternalUpdateData()
	{
		SetDirection(_direction);
		_armored = _model.ap > 0;
		_armorDecor = _armorDecor ? _armorDecor : GetDecor(Names::Armor);
		if (_armorDecor) {
			_armorDecor->SetVisible(_armored);
		}
		// else { skip }
	}

	void Target::InternalUpdatePosition()
	{
		float dx = _model.deviationX.getGlobalFrame(_t);
		float dy = _model.deviationY.getGlobalFrame(_t);

		FPoint delta = dx * _direction + dy * _across;

		SetPosition(_sourcePosition + delta);
	}

	void Target::TakeHpDamage(int value)
	{
		_model.hp -= value;
		if (_model.hp <= 0) {
			Message message(_name, "destroyed");
			message.variables.setString("type", _type);
			Core::messageManager.putMessage(message);
			_scene->RemoveObjectLater(this);

			MM::manager.PlaySample("destroyed");
			_scene->AddEffect("TargetDestroyed", GetPosition());
			// _scene->FireSignal(Signal("target_destroyed", this));
			// OR _scene->FireSignal("target_destroyed", this);
		}
		else {
			Message message(_name, "damaged", value);
			Core::messageManager.putMessage(message);

			MM::manager.PlaySample("damaged");

			// Signal signal{"target_damaged", this};
			// signal.data.setInt("value", value);
			// _scene->FireSignal(signal);
		}
	}

	void Target::TakeApDamage(int value)
	{
		_model.ap -= value;
		if (_model.ap <= 0) {
			_armored = false;
			if (_armorDecor) {
				_armorDecor->SetVisible(_armored);
			}
			// else { skip }

			Message message(_name, "armor_destroyed");
			Core::messageManager.putMessage(message);

			MM::manager.PlaySample("damaged");
			_scene->AddEffect("ArmorDestroyed", GetPosition());
			// _scene->FireSignal("target_armor_destroyed", name);
			TakeHpDamage(-_model.ap);
		}
		else {

			Message message(_name, "armor_damaged");
			Core::messageManager.putMessage(message);

			MM::manager.PlaySample("clank");

			// Signal::Ptr signal = new Signal("target_armor_damaged", this);
			// signal->GetData().setInt("value", value);
			// _scene->FireSignal(signal);
		}
	}

}
