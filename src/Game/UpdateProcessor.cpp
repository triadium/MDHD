#include "stdafx.h"
#include <unordered_set>
#include "Game/Collider.h"
#include "Game/Object.h"
#include "Game/Scene.h"
#include "Game/Processor.h"
#include "Game/UpdateProcessor.h"

namespace Game {
	
	UpdateProcessor::UpdateProcessor(Scene * scene)
		: Processor(scene)
	{
	}

	void UpdateProcessor::Update(float dt)
	{
		for (auto& object : _scene->GetObjects()) {
			object->Update(dt);
		}

		_scene->UpdateEffects(dt);
	}

	void UpdateProcessor::RemovePendingObjects(const Object::Container & objectsToBeRemoved)
	{
	}

	void UpdateProcessor::Clear()
	{
	}

}