// Copyright (C) 2019 Aleksey Khachaturov
// https://github.com/triadium/MDHD
// License: GNU GPL 3 or later, see file LICENSE

#pragma once

namespace Game {

	class Gun: public Object
	{
	public:		
		typedef boost::intrusive_ptr<Gun> Ptr;
		
		// Модель данных орудия
		struct Model {
			// Здоровье
			int hp;
			// Время на перезарядку (секунды)
			float cooldown;
			// Скорость для снаряда, проброс настроек, 
			// но само орудие должно хранить типы, а по ним строить префабы
			float bulletSpeed;
		};

		Gun(const std::string& name, Material * base);

		void Update(float dt) override;
		void MouseDown(const IPoint &mouse_pos) override;
		bool OnEnterBoundsIntersect(Object * other) override;

		const Model& GetModel() const;
		void SetModel(const Model& value);

		void TakeDamage(int value);

	private:
		void Init();
		void InternalUpdateView();
		void InternalUpdateData();
		Bullet::Ptr CreateBulletPrefab();

	private:
		Model _model;
		View::Ptr _barrelDecor;
		View::Ptr _redLampDecor;
		View::Ptr _greenLampDecor;
		// TODO: Добавить пул снарядов
		Bullet::Ptr _bulletPrefab;
		float _cooldown;
	};

}
