#include "stdafx.h"
#include <unordered_set>
#include "Game/Object.h"
#include "Game/Scene.h"
#include "Game/Processor.h"
#include "Game/SignalProcessor.h"

namespace Game {
	
	SignalProcessor::SignalProcessor(Scene * scene)
		: Processor(scene)
	{
	}

	void SignalProcessor::Update(float dt)
	{						
	}

	void SignalProcessor::RemovePendingObjects(const Object::Container & objectsToBeRemoved)
	{
	}

	void SignalProcessor::Clear()
	{
	}

}