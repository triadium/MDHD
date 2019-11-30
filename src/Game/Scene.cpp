#include "stdafx.h"
#include <unordered_map>
#include "Game/Constants.h"
#include "Game/Object.h"
#include "Game/Processor.h"
#include "Game/Scene.h"

namespace Game {

	Scene::Scene(const std::string& name, rapidxml::xml_node<>* elem)
		: Widget(name)
	{		
		width = WINDOW_WIDTH;
		height = WINDOW_HEIGHT;
	}

	void Scene::Draw()
	{
		for (Object::Ptr& obj : _objects) {
			obj->Draw();
#ifdef _DEBUG
			obj->DrawBoundsAndColliders();
#endif // _DEBUG
		}

		_effects.Draw();
	}

	void Scene::Update(float dt)
	{	
		for (auto& processor : _processors) {
			processor->Update(dt);
		}

		// В конце каждого кадра можем удалить и добавить ожидающие объекты.
		// Для только что добавленных объектов события и обработчики сигналов 
		// будут вызваны в следующем кадре.
		RemovePendingObjects();
		AddPendingObjects();
	}

	bool Scene::MouseDown(const IPoint &mouse_pos)
	{
		for (Object::Ptr& obj : _objects) {
			obj->MouseDown(mouse_pos);
		}

		return false;
	}

	// FIXME: Движение мышки будет обрабатваться в Update
	//void Scene::MouseMove(const IPoint &mouse_pos) {}

	void Scene::MouseUp(const IPoint &mouse_pos)
	{
		for (Object::Ptr& obj : _objects) {
			obj->MouseUp(mouse_pos);
		}
	}

	void Scene::AddObject(Object * object)
	{
		Assert(object != nullptr);
		Assert(object->GetScene() == nullptr);
		_objects.push_back(object);
		object->SetScene(this);
	}

	//void Scene::AddObject(Object * object, size_t index)
	//{
	//	Assert(object != nullptr);
	//	Assert(object->GetScene() == nullptr);
	//	_objects.insert(_objects.begin() + index, object);
	//	object->SetScene(this);
	//}

	void Scene::RemoveObject(Object * object)
	{
		Assert(object != nullptr);
		auto fit = std::find(_objects.begin(), _objects.end(), object);
		if (fit != _objects.end()) {
			RemoveObject(fit);
			_objects.erase(fit);
		}
		// else { noop }
	}

	const Object::Container & Scene::GetObjects() const
	{
		return _objects;
	}

	void Scene::Clear()
	{
		for (auto& processor : _processors) {
			processor->Clear();
		}

		for (auto it = _objects.begin(); it != _objects.end(); ++it) {
			RemoveObject(it);
		}
		_objects.clear();
	}

	void Scene::AddObjectLater(Object * object)
	{
		_objectsToBeAdded.push_back(object);
	}

	void Scene::AddObjectLater(const Object::Ptr & object)
	{
		_objectsToBeAdded.push_back(object);
	}

	void Scene::RemoveObjectLater(Object * object)
	{
		_objectsToBeRemoved.push_back(object);
	}

	void Scene::RemoveObjectLater(const Object::Ptr & object)
	{
		_objectsToBeRemoved.push_back(object);
	}

	ParticleEffectPtr Scene::AddEffect(const std::string & name)
	{
		ParticleEffectPtr effect = _effects.AddEffect(name);
		effect->Reset();
		return effect;
	}

	ParticleEffectPtr Scene::AddEffect(const std::string & name, const FPoint & position)
	{
		ParticleEffectPtr effect = _effects.AddEffect(name, position);
		effect->Reset();
		return effect;
	}

	void Scene::UpdateEffects(float dt)
	{
		_effects.Update(dt);
	}

	void Scene::AddProcessor(Processor::Unique&& processor)
	{
		_processors.push_back(std::move(processor));
	}

	void Scene::AddPendingObjects()
	{
		if (_objectsToBeAdded.size() > 0) {
			for (auto& object : _objectsToBeAdded) {
				AddObject(object);
			}
			_objectsToBeAdded.clear();
		}
		//else { skip }
	}

	void Scene::RemovePendingObjects()
	{
		// FIXME: Необходимо в каждый процессор сообщить, что объект удаляется со сцены
		// и необходимо очистить все внутренние ссылки на этот объект, чтобы не было проблем
		// в работе процессоров

		if (_objectsToBeRemoved.size() > 0) {
			for (auto& processor : _processors) {
				processor->RemovePendingObjects(_objectsToBeRemoved);
			}

			for (auto& object : _objectsToBeRemoved) {
				RemoveObject(object);
			}
			_objectsToBeRemoved.clear();
		}
		//else { skip }
	}

	void Scene::RemoveObject(const Object::Container::iterator & it)
	{
		(*it)->SetScene(nullptr);
	}

}