#include "stdafx.h"
#include "Game/Constants.h"
#include "Game/Material.h"
#include "Game/View.h"
#include "Game/Object.h"
#include "Game/Scene.h"
#include "Game/Target.h"
#include "Game/DuckTarget.h"
#include "Game/TargetSpawner.h"

namespace Game {

	TargetSpawner::TargetSpawner(const std::string & name, Target* prefab)
		: Object(name, (View *)nullptr)
		, _prefab(prefab)
		, _t(0)
		, _count(0)
	{
	}

	void TargetSpawner::Update(float dt)
	{
		_t += dt;
		if (_t > _model.period) {
			_t -= _model.period;

			if (_count < _model.maxCount) {
				_count++;
				auto target = Create();
				auto& position = GetPosition();
				if (position.x > _scene->getWidth() * 0.5f) {
					target->SetDirection(FPoint(-1, 0));
				}
				else {
					target->SetDirection(FPoint(1, 0));
				}
				target->SetSourcePosition(position);
				_scene->AddObjectLater(target);
			}
			else {
				_scene->RemoveObjectLater(this);
			}
		}
		// else{ skip }
	}

	const TargetSpawner::Model & TargetSpawner::GetModel() const
	{
		return _model;
	}

	void TargetSpawner::SetModel(const Model & value)
	{
		_model = value;
		InternalUpdateData();
	}

	Target::Ptr TargetSpawner::Create()
	{
		Assert(_prefab);
		// FIXME: Лучше все сделать через интерфейс клонирования,
		// так как есть шанс, что не будет известен тип префаба.
		// При использовании интерфейса клонирования можно спокойно
		// указывать более абстрактные типы и не плодить специальные 
		// фабрики.
		if (_prefab->GetType() == Names::Duck) {
			DuckTarget::Ptr target = new DuckTarget(*reinterpret_cast<DuckTarget*>(_prefab.get()));
			return target;
		}
		else {
			Target::Ptr target = new Target(*_prefab);
			return target;
		}
	}

	void TargetSpawner::InternalUpdateData()
	{
		// Чтобы сразу создал один экземпляр
		_t = _model.period;
		_count = 0;
	}

}
