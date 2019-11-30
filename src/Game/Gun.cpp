#include "stdafx.h"
#include "Game/Constants.h"
#include "Game/View.h"
#include "Game/Material.h"
#include "Game/Object.h"
#include "Game/Scene.h"
#include "Game/BoxCollider.h"
#include "Game/CircleCollider.h"
#include "Game/Bullet.h"
#include "Game/Gun.h"

namespace Game {

	Gun::Gun(const std::string & name, Material * base)
		: Object(name, base)
		, _cooldown(0.0f)
	{
		Init();
	}

	void Gun::Update(float dt)
	{
		if (_cooldown > 0) {
			_cooldown -= dt;
		}
		else {
			_redLampDecor->SetVisible(false);
			_greenLampDecor->SetVisible(true);
		}

		FPoint aimPosition = Core::mainInput.GetMousePos();
		auto& gunPosition = GetPosition();
		aimPosition -= gunPosition;
		if (aimPosition.x != 0.0f || aimPosition.y != 0.0f) {
			float barrelAngle = 180.0f * math::OneByPI  * aimPosition.GetAngle() - 90.0f;
			barrelAngle = math::clamp(-55.0f, 55.0f, barrelAngle);
			_barrelDecor->SetEulerAngle(barrelAngle);
		}
		// else{ skip }
	}

	void Gun::MouseDown(const IPoint & mouse_pos)
	{
		if (_cooldown <= 0.0f) {
			_cooldown = _model.cooldown;

			_redLampDecor->SetVisible(true);
			_greenLampDecor->SetVisible(false);

			float aimAngle = _barrelDecor->GetEulerAngle();

			FPoint aimPosition(0.0f, 1.0f);
			aimPosition.Rotate(math::PI / 180.0f * aimAngle);
			auto& gunPosition = GetPosition();

			Bullet::Ptr bullet = new Bullet(*_bulletPrefab);

			int length = _view->Height() + _barrelDecor->Height();
			auto sourcePosition = aimPosition * length + gunPosition;
			bullet->SetSourcePosition(sourcePosition);
			bullet->SetDirection(aimPosition);
			bullet->SetEulerAngle(aimAngle);

			// TODO: Сделать функционал сцены, который бы только по одному вызову метода 
			// проигрывал звук и добавлял эффект			
			MM::manager.PlaySample("shot");
			_scene->AddEffect("Shot", sourcePosition);

			_scene->AddObjectLater(bullet);
			// TODO: Сделать уведомительные функции, чтобы инициализацию проводить локально, а не внешне
			bullet->Fly(_scene);
		}
		else {
			// MM::manager.PlaySample("notyet");
		}
	}

	bool Gun::OnEnterBoundsIntersect(Object * other)
	{
		auto& otherName = other->GetName();
		if (otherName == Names::Bomb) {
			return true;
		}
		else if (otherName == Names::Target) {
			return true;
		}
		// else { noop }
		return false;
	}

	const Gun::Model & Gun::GetModel() const
	{
		return _model;
	}

	void Gun::SetModel(const Model & value)
	{
		_model = value;
		InternalUpdateData();
	}

	void Gun::TakeDamage(int value)
	{
		_model.hp -= value;
		if (_model.hp <= 0) {
			Message message(_name, "gun_destroyed");
			Core::messageManager.putMessage(message);
		}
		else {
			Message message(_name, "gun_damaged");
			Core::messageManager.putMessage(message);
			InternalUpdateView();
			_scene->AddEffect("GunDamaged", GetPosition());
		}
	}

	void Gun::Init()
	{
		// FIXME: Жесткое задание значений по умолчанию для моделей плохо
		// и это все в пользу универсального хранилища значений моделей с внешней инициализацией
		// и передачей их дальше
		_model.cooldown = 0.3f;
		_model.bulletSpeed = 390.0f;
		_bulletPrefab = CreateBulletPrefab();		
	}

	void Gun::InternalUpdateView()
	{
		auto tower75 = GetDecor(Names::Tower75);
		auto tower50 = GetDecor(Names::Tower50);
		auto tower25 = GetDecor(Names::Tower25);

		tower75->SetVisible(false);
		tower50->SetVisible(false);
		tower25->SetVisible(false);

		if (_model.hp <= 25) {
			tower25->SetVisible(true);
		}
		else if (_model.hp <= 50) {
			tower50->SetVisible(true);
		}
		else if (_model.hp <= 75) {
			tower75->SetVisible(true);
		}
		//else { noop }
	}

	void Gun::InternalUpdateData()
	{
		_cooldown = 0.0f;
		_barrelDecor = _barrelDecor ? _barrelDecor : GetDecor(Names::Barrel);
		_redLampDecor = _redLampDecor ? _redLampDecor : GetDecor(Names::RedLamp);
		_greenLampDecor = _greenLampDecor ? _greenLampDecor : GetDecor(Names::GreenLamp);

		if (_redLampDecor) {
			_redLampDecor->SetVisible(false);
		}
		// else { noop }
		if (_greenLampDecor) {
			_greenLampDecor->SetVisible(true);
		}
		// else { noop }
		
		Bullet::Model bulletModel = _bulletPrefab->GetModel();
		bulletModel.speed = _model.bulletSpeed;
		_bulletPrefab->SetModel(bulletModel);
		InternalUpdateView();
	}

	Bullet::Ptr Gun::CreateBulletPrefab()
	{
		auto bulletTexture = Core::resourceManager.Get<Render::Texture>("bullet");
		Material::Ptr bulletMaterial = new Material(bulletTexture);
		Bullet::Ptr bullet = new Bullet(Names::Bullet, bulletMaterial.get());
		float bulletWidth = bullet->GetView()->Width();
		float bulletHeight = bullet->GetView()->Height(); 
		bullet->AddCollider(FPoint(0.0f, 0.0f), std::make_unique<BoxCollider>(Names::Bullet, FPoint(bulletWidth, bulletHeight)));
		//bullet->AddCollider(FPoint(0.0f, 0.25f), std::make_unique<CircleCollider>(Names::Bullet, bulletWidth * 0.7f));
		// FIXME: Пока нет пересчета границ при повороте коллайдеров делаем границы максимально возможными для снаряда
		float bulletBoundSize = math::max(bulletWidth, bulletHeight);
		bullet->SetBounds(bulletBoundSize, bulletBoundSize);

		Bullet::Model model;
		model.power = 25;
		model.speed = _model.bulletSpeed;
		model.ricochetCount = 1;
		//
		model.speedFactor.addKey(0.0f, 0.6f);
		model.speedFactor.addKey(0.1f, 0.6f);
		model.speedFactor.addKey(1.0f, 1.0f);
		//
		model.deviationX.addKey(0.0f, 0.0f);
		//
		model.deviationY.addKey(0.0f, 0.0f);
		//model.deviationY.addKey(0.25f, 25.0f);
		//model.deviationY.addKey(0.75f, -25.0f);
		//model.deviationY.addKey(1.0f, 0.0f);

		bullet->SetModel(model);

		return bullet;
	}

}
