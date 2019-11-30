#include "stdafx.h"
#include "Game/Object.h"
#include "Game/Signal.h"

namespace Game {

	Signal::Signal(const std::string & name, Object * sourceObject)
		: _name(name)
		, _sourceObject(sourceObject)
	{
	}

	const std::string & Signal::GetName() const
	{
		return _name;
	}

	const Object::Ptr & Signal::GetSourceObject() const
	{
		return _sourceObject;
	}

	VariableSet & Signal::GetData() const
	{
		return _data;
	}

}
