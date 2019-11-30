// Copyright (C) 2019 Aleksey Khachaturov
// https://github.com/triadium/MDHD
// License: GNU GPL 3 or later, see file LICENSE

#pragma once

namespace Game {

	// Представление на сцене служит для сохранения и обновления текущего 
	// локального состояния трансформации. У каждого представления есть 
	// точка поворота, коэффициенты масштабирования по осям XY и угол 
	// поворота в плоскости экрана (вокруг оси Z).
	// Прорисовка представления происходит с помощью назначенного материала.
	// Точка поворота рассчитывается как отклонение от начала локальных координат.
	// Начало локальных координат и отклонения задаются в нормализованных единицах, 
	// которые определяются на основании ширины и высоты  материала.
	// Изначально нулевые локальные координаты находятся в нижнем левом углу,
	// но по умолчанию они сдвигаются в центр представления.
	// Доступ к значениям точек, отклонений, материалу, размерам осуществляется через
	// Get*/Set* функции, а не напрямую, так как есть необходимость обновлять
	// внутреннее состояние представления и не заставлять потребителя
	// вызывать функцию обновления вручную.
	
	class View: public RefCounter
	{
	public:
		typedef boost::intrusive_ptr<View> Ptr;
		typedef std::vector<Ptr> Container;

		View(const std::string& name, Material * material);
		//View(const View& view);

		void* operator new(size_t i) { return _mm_malloc(i, 16); }
		void operator delete(void* p) { _mm_free(p); }

		const std::string& GetName() const;
		bool IsVisible();
		void SetVisible(bool value);

		void Draw(const FPoint& position);
		void Draw(float x, float y) { Draw(FPoint(x, y)); }
		void DrawIntermediate(const FPoint& position);
		void DrawIntermediate(float x, float y) { DrawIntermediate(FPoint(x, y)); }

		// TODO: Перенести в отдельный класс Трасформация. 
		// Будет использоваться как для представления на сцене, так и для 
		// представлений Процессору обработки столкновений (коллайдеров).
		// TODO: Так как появился Материал и теперь Представления клонируются, 
		// то точку привязки к родителю перенести в класс Трансформация,
		// а для Объекта убрать установку Представлений извне.
		const FPoint& GetOrigin() const;
		void SetOrigin(const FPoint& value);
		void SetOrigin(float x, float y) { SetOrigin(FPoint(x, y)); }

		const FPoint& GetPivotOffset() const;
		void SetPivotOffset(const FPoint& value);
		void SetPivotOffset(float x, float y) { SetPivotOffset(FPoint(x, y)); }

		const FPoint& GetScale() const;
		void SetScale(const FPoint& value);
		void SetScale(float x, float y) { SetScale(FPoint(x, y)); }

		// Направление представления -1 | +1
		const IPoint& GetFlips() const;
		// Можно указать 0 для измерения, чтобы не менять направление по нему
		void SetFlips(int x, int y);
		void SetFlips(const IPoint& flips) { SetFlips(flips.x, flips.y); }
		// "Истина" для измерения - сменить направление для измерения
		void Flip(bool x, bool y);

		float GetEulerAngle() const;
		void SetEulerAngle(float value);

		Material * GetMaterial() const;
		void SetMaterial(Material * value);

		int Width() const;
		int Height() const;

		FPoint GetPin(const FPoint& offset) const;

		const math::Matrix4& GetLocalTransform() const;

	private:
		void InternalUpdate();

	private:
		std::string _name;
		Material::Ptr _material;
		IPoint _flips;
		FPoint _origin;
		FPoint _pivotOffset;
		FPoint _scale;
		float _eulerAngle;
		FPoint _pivot;
		bool _isVisible;
		math::Matrix4 _localTransform;
	};
}
