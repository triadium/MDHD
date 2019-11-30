#include "stdafx.h"
#include "Game/Collider.h"
#include "Game/Material.h"
#include "Game/View.h"
#include "Game/Scene.h"
#include "Game/Object.h"

namespace Game {

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

	Object::Object(const std::string & name)
		: _name(name)
		, _view(nullptr)
		, _flips(1, 1)
		, _scene(nullptr)
		, _isActive(true)
		, _position(FPoint(0.0f, 0.0f))
		, _scale(FPoint(1.0f, 1.0f))
		, _bounds(FRect(0.0f, 0.0f, 0.0f, 0.0f))
	{
	}
	
	Object::Object(const std::string& name, View * base)
		: _name(name)
		, _view(base)
		, _flips(1, 1)
		, _scene(nullptr)
		, _isActive(true)
		, _position(FPoint(0.0f, 0.0f))
		, _scale(FPoint(1.0f, 1.0f))
		, _bounds(FRect(0.0f, 0.0f, 0.0f, 0.0f))
	{
		_scale = (_view ? _view->GetScale() : FPoint(1.0f, 1.0f));
	}

	Object::Object(const std::string & name, Material * base)
		: _name(name)
		, _view(new View(name, base))
		, _flips(1, 1)
		, _scene(nullptr)
		, _isActive(true)
		, _position(FPoint(0.0f, 0.0f))
		, _scale(FPoint(1.0f, 1.0f))
		, _bounds(FRect(0.0f, 0.0f, 0.0f, 0.0f))

	{
	}

	Object::Object(const Object & obj)
		: RefCounter(obj)
	{
		Copy(obj);
	}

	Object & Object::operator=(const Object & obj)
	{
		if (this != &obj) {
			Copy(obj);
		}
		// else { noop }

		return *this;
	}

	Object::~Object()
	{
		_decorPins.clear();
		_decors.clear();
		_colliders.clear();
	}

	const std::string & Object::GetName() const
	{
		return _name;
	}

	bool Object::IsActive()
	{
		return _scene && _isActive;
	}

	void Object::SetActive(bool value)
	{
		_isActive = value;
	}

	Scene * Object::GetScene() const
	{
		return _scene;
	}

	void Object::SetScene(Scene * value)
	{
		_scene = value;
	}

	void Object::Draw()
	{		
		// Базовое представление задает родительскую трансформацию для декораторов
		if (_view) {
			Render::device.PushMatrix();
			_view->DrawIntermediate(_position);

			size_t index = 0;
			for (auto it = _decors.cbegin(); it != _decors.cend(); ++it, ++index) {
				auto& pin = _decorPins[index];
				(*it)->Draw(pin);
			}
			Render::device.PopMatrix();
		}
		// FIXME: Не имеет смысла выводить дочерние представления, 
		// если родительское не задано
		//else {
		//	//Без базового представления выводим декораторы в позиции самого объекта
		//	size_t index = 0;
		//	for (auto it = _decors.cbegin(); it != _decors.cend(); ++it, ++index) {
		//		auto& pin = _decorPins[index];
		//		(*it)->Draw(_position.x + pin.x, _position.y + pin.y);
		//	}
		//}
	}

	View * Object::GetView() const
	{
		return _view.get();
	}

	void Object::SetView(View * base)
	{
		_view = base;
		_scale = (_view ? _view->GetScale() : FPoint(1.0f, 1.0f));
	}

	const View::Container & Object::GetDecors() const
	{
		return _decors;
	}

	void Object::AddDecor(const FPoint & pinOffset, View * decor)
	{
		Assert(decor != nullptr);
		_decors.push_back(decor);
		_decorPins.push_back(GetPin(pinOffset));
	}

	void Object::AddDecor(const FPoint & pinOffset, View * decor, size_t index)
	{		
		Assert(decor != nullptr);
		_decors.insert(_decors.begin() + index, decor);
		_decorPins.insert(_decorPins.begin() + index, GetPin(pinOffset));
	}

	View * Object::AddDecor(const FPoint & pinOffset, Material * decor)
	{
		View::Ptr view = new View(_name, decor);
		AddDecor(pinOffset, view.get());
		return view.get();
	}

	View * Object::AddDecor(const FPoint & pinOffset, Material * decor, size_t index)
	{
		View::Ptr view = new View(_name, decor);
		AddDecor(pinOffset, view.get(), index);
		return view.get();
	}

	void Object::RemoveDecor(View * decor)
	{
		auto fit = std::find(_decors.begin(), _decors.end(), decor);
		if (fit != _decors.end()) {
			_decors.erase(fit);
			size_t index = fit - _decors.begin();
			_decorPins.erase(_decorPins.begin() + index);
		}
		// else { noop }
	}

	View * Object::GetDecor(const std::string & name) const
	{
		for (auto& view : _decors) {
			if (view->GetName() == name) {
				return view.get();
			}
			// else { skip }
		}
		return nullptr;
	}

	const FPoint & Object::GetDecorPin(size_t index) const
	{
		Assert(index < _decors.size());
		return _decorPins[index];
	}

	const FPoint & Object::GetPosition() const
	{
		return _position;
	}

	void Object::SetPosition(const FPoint & value)
	{
		_position = value;
		InternalUpdateAabb();
	}

	float Object::GetEulerAngle() const
	{
		return _view ? _view->GetEulerAngle() : 0.0f;
	}

	void Object::SetEulerAngle(const float value)
	{
		if (_view) {
			_view->SetEulerAngle(value);
			// TODO: Пока не сделали обновление размеров границ
			// не обновляем и установленные вручную границы при повороте
			// InternalUpdateBounds();
			// InternalUpdateAabb();
		}
		// else { noop }
	}

	// FIXME: Для сохранения контракта, по которому прямое изменение
	// значений трансформации запрещено, добавили кэширующее значение
	const FPoint & Object::GetScale() const
	{
		return _scale;
	}

	void Object::SetScale(const FPoint & value)
	{
		_scale = value;
		if (_view) {
			_view->SetScale(value);
			InternalUpdateBounds();
			InternalUpdateAabb();
		}
		// else { noop }
	}

	const IPoint & Object::GetFlips() const
	{
		return _flips;
	}

	void Object::SetFlips(int x, int y)
	{
		_flips.x = x == 0 ? _flips.x : (x > 0 ? 1 : -1);
		_flips.y = y == 0 ? _flips.y : (y > 0 ? 1 : -1);
	}

	void Object::Flip(bool x, bool y)
	{
		_flips.x *= (x ? -1 : 1);
		_flips.y *= (y ? -1 : 1);

		if (_view) {
			_view->Flip(x, y);
		}
		// else { skip }
	}

	const FRect & Object::GetBounds() const
	{
		return _bounds;
	}

	void Object::SetBounds(const FRect & value)
	{
		_bounds = value;
		InternalUpdateAabb();
	}

	const FRect & Object::GetAabb() const
	{
		return _aabb;
	}

	const Collider::Container & Object::GetColliders() const
	{
		return _colliders;
	}

	void Object::AddCollider(const FPoint& pinOffset, Collider::Unique&& collider)
	{
		_colliders.push_back(std::move(collider));
		_colliderPins.push_back(GetPin(pinOffset));
		InternalUpdateBounds();
		InternalUpdateAabb();
	}

	void Object::UpdateColliders()
	{
		if (_view) {
			Render::device.PushMatrix();
			Render::device.MatrixTranslate(_position);
			Render::device.MatrixMultiply(_view->GetLocalTransform());

			size_t index = 0;
			for (auto it = _colliders.cbegin(); it != _colliders.cend(); ++it, ++index) {
				auto& pin = _colliderPins[index];
				(*it)->Update(pin);
			}

			Render::device.PopMatrix();
		}
		// else { skip }
	}

	//void Object::RemoveCollider(Collider * collider)
	//{
	//}

	bool Object::OnEnterBoundsIntersect(Object * other)
	{
		return false;
	}

	void Object::OnEnterCollision(Collider * collider, Object * other, Collider * otherCollider)
	{
	}

	void Object::Update(float dt)
	{
		// TODO: Назначать интерфейс Updatable и потом использовать через dynamic_cast 
		// для проверки наличия интерфейса
	}

	void Object::MouseDown(const IPoint & mouse_pos)
	{
	}

	void Object::MouseUp(const IPoint & mouse_pos)
	{
	}

	FPoint Object::GetPin(const FPoint & offset) const
	{
		return _view ? _view->GetPin(offset) : FPoint();
	}

	void Object::Copy(const Object & obj)
	{
		// Новый объект не привязан к сцене
		_scene = nullptr;
		_name = obj._name;

		// Создаем отдельный экземпляр базового представления, если установлено
		_view = obj._view ? new View(*obj._view) : nullptr;
		_isActive = obj._isActive;
		_position = obj._position;
		_scale = obj._scale;
		_flips = obj._flips;
		_bounds = obj._bounds;
		_decorPins = obj._decorPins;
		_colliderPins = obj._colliderPins;

		// Создаем копии декора
		for (auto& decor : obj._decors) {
			_decors.push_back(new View(*decor));
		}

		// Создаем копии через метод клонирования, так как коллайдеры могут быть разных типов
		for (auto& collider : obj._colliders) {
			_colliders.push_back(std::move(Collider::Unique(collider->Clone())));
		}
	}

	void Object::InternalUpdateBounds()
	{
		// TODO: Сделать расчет размеров границ объекта на основании
		// текущей трансформации коллайдеров 
		// TODO: Кэшировать матрицу трансформации коллайдеров 
		// для быстрого обновления их данных

		if (_colliders.size() > 0 && _view) {
			int width = _view->Width();
			int height = _view->Height();
			_bounds.xStart = 0;
			_bounds.yStart = 0;
			_bounds.xEnd = width;
			_bounds.yEnd = height;
		}
		// Не меняем границы, потому что они могут быть выставлены вручную
		// else { skip }
	}

	void Object::InternalUpdateAabb()
	{
		float width = _bounds.Width();
		float height = _bounds.Height();
		_aabb = _bounds.MovedTo(FPoint(_position.x - 0.5f * width, _position.y - 0.5f * height));
	}

#ifdef _DEBUG
	void Object::DrawBoundsAndColliders()
	{
		if (_bounds.Width() > 0) {
			Render::device.PushMatrix();
			Render::device.SetTexturing(false);
			Render::BeginAlphaMul(0.39f);

			Render::BeginColor(Color(255, 128, 128, 255));
			Render::DrawRect(_aabb.Rounded());
			Render::EndColor();

			if (_view) {
				Render::device.MatrixTranslate(_position);
				Render::device.MatrixMultiply(_view->GetLocalTransform());
				Render::BeginColor(Color(128, 255, 128, 255));
				size_t index = 0;
				for (auto it = _colliders.cbegin(); it != _colliders.cend(); ++it, ++index) {
					auto& pin = _colliderPins[index];
					(*it)->Draw(pin);
				}
				Render::EndColor();
			}
			// else { skip }


			Render::EndAlphaMul();
			Render::device.SetTexturing(true);
			Render::device.PopMatrix();
		}
		// else { skip }
	}
#endif // _DEBUG

}
