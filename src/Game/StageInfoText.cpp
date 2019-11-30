#include "stdafx.h"
#include "Game/Material.h"
#include "Game/View.h"
#include "Game/Object.h"
#include "Game/Scene.h"
#include "Game/StageInfoText.h"

namespace Game {

	StageInfoText::StageInfoText(const std::string & name, const std::string & fontName)
		: Object(name, (View *)nullptr)
		, _fontName(fontName)
		, _direction(1.0f, 0.0f)
		, _speed(0.0f, 0.0f)
		, _isRunning(true)
	{
	}

	void StageInfoText::Draw()
	{
		if (Render::isFontLoaded(_fontName)) {
			Render::device.PushMatrix();
			auto& scale = GetScale();
			Render::device.MatrixScale(scale.x, scale.y, 1.0f);
			Render::BindFont(_fontName);

			Render::PrintString(GetPosition(), _model.text, 1.0, CenterAlign, CenterAlign);

			Render::device.PopMatrix();
		}
		// else { skip }
	}

	void StageInfoText::Update(float dt)
	{
		_model.showTime -= dt;

		if (_model.showTime <= 0.0f) {
			_scene->RemoveObjectLater(this);
		}
		// else { noop }

		if (_isRunning) {
			auto position = GetPosition();
			if (math::abs(position.x - _model.endPoint.x) <= _speed.x * dt && 
				math::abs(position.y - _model.endPoint.y) <= _speed.y * dt) {
				SetPosition(_model.endPoint);
				_isRunning = false;
			}
			else {
				SetPosition(position + _direction * _speed * dt);
			}
		}
		// else{ noop }
	}

	const StageInfoText::Model & StageInfoText::GetModel() const
	{
		return _model;
	}

	void StageInfoText::SetModel(const Model & value)
	{
		_model = value;
		InternalUpdateData();
	}

	void StageInfoText::InternalUpdateData()
	{
		Assert(_model.time > 0.0f);
		_isRunning = true;
		SetPosition(_model.startPoint);
		_speed = _model.endPoint - _model.startPoint;
		_direction = _speed.Normalized();

		_speed.x = math::abs(_speed.x / _model.time);
		_speed.y = math::abs(_speed.y / _model.time);
	}

}
