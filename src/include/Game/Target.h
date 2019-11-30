// Copyright (C) 2019 Aleksey Khachaturov
// https://github.com/triadium/MDHD
// License: GNU GPL 3 or later, see file LICENSE

#pragma once

namespace Game {

	class Target: public Object
	{
	public:		
		typedef boost::intrusive_ptr<Target> Ptr;
		typedef std::vector<Ptr> Container;
		
		// FIXME: Можно создать в Игровом Объекте модель данных,
		// которая будет представлять собой VariableSet.
		// Это увеличит гибкость при работе (не нужно делать cast), 
		// но снизит производительность и компактность.
		// Модель данных цели
		struct Model {
			// Урон, который нанесет цель, если врежется в пушку
			int power;
			// Здоровье
			int hp;
			// Броня
			int ap;
			// Скорость (px/sec)
			float speed;
			// Множитель скорости для одной секунды передвижения
			TimedSpline<float> speedFactor;
			// Отклонение по оси х для одной секунды передвижения
			TimedSpline<float> deviationX;
			// Отклонение по оси y для одной секунды передвижения
			TimedSpline<float> deviationY;
		};

		Target(const std::string & type, const std::string& name, Material * base);
		explicit Target(const Target & obj);
		Target& operator=(const Target& obj);
		~Target() override;

		const std::string& GetType() const;

		void Update(float dt) override;

		const Model& GetModel() const;
		void SetModel(const Model& value);

		const FPoint& GetSourcePosition() const;
		void SetSourcePosition(const FPoint& value);
		void SetSourcePosition(float x, float y) { SetSourcePosition(FPoint(x, y)); }

		const FPoint& GetDirection() const;
		void SetDirection(const FPoint& value);

		void FlipDirection();
		virtual void TakeDamage(int value);

	protected:
		void Copy(const Target& obj);

		void Explode();

	private:
		void InternalUpdateData();
		void InternalUpdatePosition();
		void TakeHpDamage(int value);
		void TakeApDamage(int value);

	private:
		// Тип цели
		// FIXME: Простой способ выбора класса для фабрик
		// Можно сделать и через функторы и даже через ECS, 
		// но мало времени сейчас на реализацию и оптимизацию
		std::string _type;

		// Независимые значения
		Model _model;
		FPoint _direction;
		FPoint _sourcePosition;
		float _t;

		// Вычисляемые значения
		bool _armored;
		FPoint _across;
		FPoint _velocity;
		View::Ptr _armorDecor;
	};

}
