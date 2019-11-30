#include "stdafx.h"
#include "Game/Material.h"
#include "Game/View.h"
#include "Game/Object.h"
#include "Game/Scene.h"
#include "Game/StageTimeCountdown.h"

namespace Game {

	// TODO: Сделать отдельное представление для надписей
	StageTimeCountdown::StageTimeCountdown(const std::string & name, const std::string & fontName)
		: Object(name, (View *)nullptr)
		, _fontName(fontName)
		, _t(0)
		, _wt(0)
		, _tickSoundId(-1)
	{
		_offset.x = Render::getStringWidth("Time: 000", _fontName);
		_offset.y = Render::getFontHeight(_fontName);
	}

	void StageTimeCountdown::Draw()
	{
		if (Render::isFontLoaded(_fontName)) {
			Render::device.PushMatrix();
			auto& scale = GetScale();
			Render::device.MatrixScale(scale.x, scale.y, 1.0f);
			Render::BindFont(_fontName);
			Render::PrintString(GetPosition() - _offset, std::string("Time: ") + utils::lexical_cast(_t, 0), 1.0, LeftAlign, CenterAlign);

			Render::device.PopMatrix();
		}
		// else { skip }
	}

	void StageTimeCountdown::Update(float dt)
	{
		_t -= dt;

		if (_t < 0) {
			MM::manager.StopSample(_tickSoundId);
			_tickSoundId = -1;

			_scene->RemoveObjectLater(this);
			Message message(_name, "done");
			message.SetTargetLayer(_scene->getLayerName());
			Core::messageManager.putMessage(message);
		}
		else if (_model.warningTime > _t) {

			if (_tickSoundId < 0) {
				MM::manager.FadeOutTrack(1.0f);
				_tickSoundId = MM::manager.PlaySample("tick", true, 2.5f);
			}
			// else { skip }

			_wt += dt;
			if (_wt >= 1.0f) {
				_wt -= 1.0f;
			}
			// else { skip }
			float sf = _model.scaleFactor.getGlobalFrame(_wt);
			SetScale(sf, sf);
		}
		// else { skip }
	}

	const StageTimeCountdown::Model & StageTimeCountdown::GetModel() const
	{
		return _model;
	}

	void StageTimeCountdown::SetModel(const Model & value)
	{
		_model = value;
		InternalUpdateData();
	}

	void StageTimeCountdown::InternalUpdateData()
	{
		_t = _model.countdown;
		_wt = 0;
	}

}
