// Copyright (C) 2019 Aleksey Khachaturov
// https://github.com/triadium/MDHD
// License: GNU GPL 3 or later, see file LICENSE

#pragma once

namespace Game {

	class Scene;

	// Объект сцены является фасадом для различных механизмов взаимодействия:
	// игровой логики, системы определения столкновений, представления на экране
	// Для упращения будем считать, что у каждого объекта есть одно базовое
	// представление и некоторый набор дочерних декораторов. Всегда сначала выводится 
	// базовое представление, а потом уже по порядку все декораторы.
	// При необходимости можно будет расширить декораторы до двух списков, чтобы базовое
	// представление оказалось посередине. Точка присоединения декоратора указывается
	// как отклонение от точки вращения базового представления в нормализованных единицах.
	// Локальная трансформация базового представления является родительской для всех декораторов.
	// (Нормализованные единцы рассчитываются на основании значений ширины и высоты 
	// базового представления)
	// Объект может содержать набор колайдеров, по которым определяется AABB 
	// с учетом локальной трансформации для системы определения столкновений. 
	// Если колайдеров нет у объекта, то такой объект не участвует в механизмах 
	// определения столкновений и является декором сцены.
	// Вращение колайдеров производится вокруг точки вращения базового представления
	// Координаты для объекта задаются в единицах экрана, а не в нормализованных как 
	// для точки вращения и отклонения в представлении.
	// Объект может быть привязан к сцене, а может быть удален со сцены.
	// Объект может стать неактивным и тогда процессоры сцены не обрабатывают его.
	// При использовании активации/деактивации сохраняется порядок объектов сцены
	// и не происходит перераспределение массивов.
	// В дальнейшем необходимо отделить объект от логики его обновления, так как
	// неприсоединенный объект в функции обновления может обращаться к сцене.

	class Object: public RefCounter
	{
	public:
		typedef boost::intrusive_ptr<Object> Ptr;
		typedef std::vector<Ptr> Container;

		// Пустой объект. Можно установить границы для участия в системе столкновений
		Object(const std::string& name);
		Object(const std::string& name, View * base);
		Object(const std::string& name, Material * base);
		
		explicit Object(const Object & obj);
		Object& operator=(const Object& obj);
		~Object() override;

		const std::string& GetName() const;

		bool IsActive();
		void SetActive(bool value);		

		Scene * GetScene() const;
		void SetScene(Scene * value);

		virtual void Draw();

		View * GetView() const;
		void SetView(View * base);

		const View::Container& GetDecors() const;
		void AddDecor(const FPoint& pinOffset, View * decor);
		void AddDecor(const FPoint& pinOffset, View * decor, size_t index);
		View * AddDecor(const FPoint& pinOffset, Material * decor);
		View * AddDecor(const FPoint& pinOffset, Material * decor, size_t index);
		void RemoveDecor(View * decor);
		View * GetDecor(const std::string& name) const;
		const FPoint& GetDecorPin(size_t index) const;

		const FPoint& GetPosition() const;
		void SetPosition(const FPoint& value);
		void SetPosition(float x, float y) { SetPosition(FPoint(x, y)); }

		float GetEulerAngle() const;
		void SetEulerAngle(const float value);

		const FPoint& GetScale() const;
		void SetScale(const FPoint& value);
		void SetScale(float x, float y) { SetScale(FPoint(x, y)); }

		// Направление объекта -1 | +1
		const IPoint& GetFlips() const;
		// Определяет направление всего объекта независимо от 
		// направления базового вида и декораторов
		// Можно указать 0 для измерения, чтобы не менять направление по нему
		void SetFlips(int x, int y);
		void SetFlips(const IPoint& flips) { SetFlips(flips.x, flips.y); }
		// "Истина" для измерения - сменить направление для измерения
		void Flip(bool x, bool y);

		const FRect& GetBounds() const;
		void SetBounds(const FRect& value);
		void SetBounds(const FPoint& value) { SetBounds(FRect(0.0f, value.x, 0.0f, value.y)); }
		void SetBounds(float width, float height) { SetBounds(FRect(0.0f, width, 0.0f, height)); }
		const FRect& GetAabb() const;
		const Collider::Container& GetColliders() const;
		void AddCollider(const FPoint& pinOffset, Collider::Unique&& collider);
		// void RemoveCollider(Collider * collider);
		void UpdateColliders();

		// Если не нужно проверять коллайдеры для данной пары объектов,
		// то верните false
		virtual bool OnEnterBoundsIntersect(Object * other);
		virtual void OnEnterCollision(Collider* collider, Object * other, Collider* otherCollider);

		virtual void Update(float dt);

		virtual void MouseDown(const IPoint &mouse_pos);
		virtual void MouseUp(const IPoint& mouse_pos);

#ifdef _DEBUG
		void DrawBoundsAndColliders();
#endif // _DEBUG

	protected:
		FPoint GetPin(const FPoint & offset) const;
		void Copy(const Object& obj);

	protected:
		Scene * _scene;
		View::Ptr _view;
		std::string _name;

	private:
		void InternalUpdateBounds();
		void InternalUpdateAabb();

	private:
		bool _isActive;
		FPoint _position;
		FPoint _scale;
		IPoint _flips;
		View::Container _decors;
		std::vector<FPoint> _decorPins;
		// Границы в локальной системе
		FRect _bounds;
		// Границы, смещенные на позицию
		FRect _aabb;
		Collider::Container _colliders;
		std::vector<FPoint> _colliderPins;
	};

}
