#include "stdafx.h"
#include "Game/Constants.h"
#include "Game/View.h"
#include "Game/Material.h"
#include "Game/Object.h"
#include "Game/Scene.h"
#include "Game/Bullet.h"

#include "Game/Collider.h"
#include "Game/Target.h"

namespace Game {

	Bullet::Bullet(const std::string & name, Material * base)
		: Object(name, base)
		, _direction(FPoint(0.0f, 1.0f))
		, _across(FPoint(-1.0f, 0.0f))
		, _st(0)
		, _dt(0)
		, _ricochetCountdown(1)
		, _jetEffect(nullptr)
	{
	}

	Bullet::Bullet(const Bullet & obj)
		: Object(obj)
	{
		Copy(obj);
	}

	Bullet & Bullet::operator=(const Bullet & obj)
	{
		if (this != &obj) {
			Object::Copy(obj);
			Copy(obj);
		}
		// else { noop }		

		return *this;
	}

	Bullet::~Bullet()
	{
		if (_jetEffect) {
			_jetEffect->Finish();
			_jetEffect = nullptr;
		}
		// else { noop }
	}

	void Bullet::Update(float dt)
	{
		if (_st >= 1.0f) {
			_st = 1.0f;
		}
		else {
			_st += dt;
		}

		_dt += dt;
		if (_dt >= 1.0f) {
			_dt -= 1.0f;
		}
		// else { noop }

		float sf = _model.speedFactor.getGlobalFrame(_st);
		_sourcePosition += (_velocity * dt * sf);

		InternalUpdatePosition();

		_jetEffect->SetPos(GetPosition() - (_direction * _view->Height() * 0.5f));
	}

	bool Bullet::OnEnterBoundsIntersect(Object * other)
	{
		// FIXME: Во "взрослых физических движках" используются матрицы столкновений.
		auto& otherName = other->GetName();
		if (otherName == Names::Bullet) {
			return false;
		}
		else if (otherName == Names::Wall) {
			if (_ricochetCountdown > 0) {
				_ricochetCountdown--;
				
				MM::manager.PlaySample("clank");
				_scene->AddEffect("Clank", GetPosition());
				auto& wallAabb = other->GetAabb();
				if (wallAabb.Width() > wallAabb.Height()) {
					_direction.Scale(1.0f, -1.0f);
				}
				else {
					_direction.Scale(-1.0f, 1.0f);
				}
				SetDirection(_direction);
				float angle = _direction.GetAngle() * 180.0f * math::OneByPI - 90.0f;
				SetEulerAngle(angle);
			}
			else {
				Message message(_name, "missed", 0);
				Core::messageManager.putMessage(message);
				
				Explode();
			}

			return false;
		}
		else if (otherName == Names::Target) {
			return true;
		}
		else if (otherName == Names::Bomb) {
			return true;
		}
		return false;
	}

	void Bullet::OnEnterCollision(Collider * collider, Object * other, Collider * otherCollider)
	{
		if (other->GetName() == Names::Target) {
			Target * target = reinterpret_cast<Target*>(other);
			if (otherCollider->GetName() == Names::Body) {
				Explode();
				target->TakeDamage(_model.power);
			}
			else if (otherCollider->GetName() == Names::Head) {

				// FIXME: move to TakeDamage function?
				Message message(_name, "headshot", _model.power * 10);
				Core::messageManager.putMessage(message);

				Explode();
				MM::manager.PlaySample("headshot");
				_scene->AddEffect("Headshot", GetPosition());			
				// TODO: Добавить в модель коэффициент за попадание в голову
				target->TakeDamage(_model.power * 10);
			}
			// else { skip }
		}
		// else { skip }
	}

	const Bullet::Model & Bullet::GetModel() const
	{
		return _model;
	}

	void Bullet::SetModel(const Model & value)
	{
		_model = value;
		InternalUpdateData();
	}

	const FPoint & Bullet::GetSourcePosition() const
	{
		return _sourcePosition;
	}

	void Bullet::SetSourcePosition(const FPoint & value)
	{
		_sourcePosition = value;
		InternalUpdatePosition();
	}

	const FPoint & Bullet::GetDirection() const
	{
		return _direction;
	}

	void Bullet::SetDirection(const FPoint & value)
	{
		_direction = value.Normalized();
		_across = _direction.Rotated(math::PI / 2);
		_velocity = _direction * _model.speed;
	}

	// FIXME: Пока нет уведомительных функций, приходится передавать указатель на сцену прямо
	void Bullet::Fly(Scene * scene)
	{
		if (_jetEffect) {
			_jetEffect->Finish();
		}
		// else { noop }
		_jetEffect = scene->AddEffect("Jet", _sourcePosition);
	}

	void Bullet::Copy(const Bullet & obj)
	{
		_model = obj._model;
		_sourcePosition = obj._sourcePosition;
		_direction = obj._direction;
		_st = 0.0f;
		_dt = 0.0f;
		_jetEffect = nullptr;
		InternalUpdateData();
	}

	void Bullet::InternalUpdateData()
	{
		_ricochetCountdown = _model.ricochetCount;
		SetDirection(_direction);
	}

	void Bullet::InternalUpdatePosition()
	{
		float dx = _model.deviationX.getGlobalFrame(_dt);
		float dy = _model.deviationY.getGlobalFrame(_dt);
		FPoint delta = dx * _direction + dy * _across;
		SetPosition(_sourcePosition + delta);
	}

	void Bullet::Explode()
	{
		if (_jetEffect) {
			_jetEffect->Finish();
			_jetEffect = nullptr;
		}
		else { 
			_jetEffect = nullptr;
		}
		MM::manager.PlaySample("boom");
		_scene->AddEffect("Explosion", GetPosition());
		SetActive(false);
		_scene->RemoveObjectLater(this);
	}
}
