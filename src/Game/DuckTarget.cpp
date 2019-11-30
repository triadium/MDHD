#include "stdafx.h"
#include "Game/Constants.h"
#include "Game/View.h"
#include "Game/Material.h"
#include "Game/Object.h"
#include "Game/View.h"
#include "Game/Signal.h"
#include "Game/Target.h"
#include "Game/EggBomb.h"
#include "Game/DuckTarget.h"

#include <Game/Scene.h>

namespace Game {

	DuckTarget::DuckTarget(EggBomb * eggBombPrefab, Material * base)
		: Target(Names::Duck, Names::Target, base)
		, _eggBombPrefab(eggBombPrefab)
	{
	}

	DuckTarget::DuckTarget(const DuckTarget & obj)
		: Target(obj)
	{
		Copy(obj);
	}

	DuckTarget & DuckTarget::operator=(const DuckTarget & obj)
	{
		if (this != &obj) {
			Target::Copy(obj);
			Copy(obj);
		}
		// else { noop }		

		return *this;
	}

	DuckTarget::~DuckTarget()
	{
	}

	bool DuckTarget::OnEnterBoundsIntersect(Object * other)
	{
		// FIXME: Лучше завести перечисление типов объектов в игре и использовать его вместо имени
		// для осуществления приведения типов.
		// Недостаток перечисления - при изменении перечисления типов придется делать новый билд,
		// даже если логика игры зашита в lua скриптах.
		// Либо создать единый универсальный интерфейс доступа к свойствам модели игрового объекта
		// и обращаться к значению по имени (строкую константу)
		if (other->GetName() != Names::Bullet && other->GetName() != Names::Bomb) {
			this->FlipDirection();
			return false;
		}
		else {
			// V/V
			// auto bullet = reinterpret_cast<IBullet*>(other);
			// TakeDamage(bullet->GetModel().power);
			return true;
		}
	}

	void DuckTarget::TakeDamage(int value)
	{
		if (_eggBombPrefab) {

			auto& model = GetModel();
			int preHp = model.hp;
			Target::TakeDamage(value);
			if ((preHp - model.hp) > 0) {
				EggBomb::Ptr bomb = new EggBomb(*_eggBombPrefab);
				auto& position = GetPosition();
				bomb->SetSourcePosition(position);
				_scene->AddObjectLater(bomb);
			}
			//else { noop }
		}
		else {
			Target::TakeDamage(value);
		}
	}

	void DuckTarget::Copy(const DuckTarget & obj)
	{
		_eggBombPrefab = obj._eggBombPrefab;
		//InternalUpdateData();
	}
}
