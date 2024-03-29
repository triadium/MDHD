#include "stdafx.h"
#include "Game/Constants.h"
#include "Game/View.h"
#include "Game/Material.h"
#include "Game/Object.h"
#include "Game/Scene.h"
#include "Game/StageInfoText.h"
#include "Game/IntroAnimator.h"

namespace Game {

	IntroAnimator::IntroAnimator(const std::string & name)
		: Object(name, (View *) nullptr)
		, _time(0.0f)
		, _duckOffset(0.0f, 0.0f)
	{
		Init();
	}

	void IntroAnimator::Update(float dt)
	{
		if (_timeToShowCaptions <= 0.0f) {
			_timeToShowCaptions = INT_MAX;

			MM::manager.PlaySample("intro");

			auto eyeTexture = Core::resourceManager.Get<Render::Texture>("duck_eye_big");
			Material::Ptr eyeMaterial = new Material(eyeTexture);
			_bigDuckLeft = new Object("eye", eyeMaterial.get());
			//_bigDuckLeft->SetPosition(GetPosition() + FPoint(-40.0f, 300.0f));
			_scene->AddObject(_bigDuckLeft);
			_bigDuckRight = new Object(*_bigDuckLeft);
			//_bigDuckRight->SetPosition(GetPosition() + FPoint(40.0f, 300.0f));
			_scene->AddObject(_bigDuckRight);

			ShowInfoText("Voiceover: Arthur Pirozhkov - Cyborgs", 7.0f);
		}
		else {
			_timeToShowCaptions -= dt;
		}

		if (_timeToIntroShow <= 0.0f) {
			_intro = _scene->AddEffect("Intro", GetPosition());
			_timeToIntroShow = INT_MAX;

			ShowInfoText("Music: Brad Fiedel - Terminator Theme", 15.0f);
		}
		else {
			_timeToIntroShow -= dt;
		}

		if (_timeToComplete <= 0.0f) {
			Message message(_name, "completed");
			Core::messageManager.putMessage(message);
			SetActive(false);
		}
		else {
			_timeToComplete -= dt;
		}

		if (_timeToShowDucks <= 0.0f) {
			auto duckBigTexture = Core::resourceManager.Get<Render::Texture>("duck_big");
			Material::Ptr duckBigMaterial = new Material(duckBigTexture);
			_bigDuckLeft->GetView()->SetMaterial(duckBigMaterial.get());
			_bigDuckRight->GetView()->SetMaterial(duckBigMaterial.get());
			_duckOffset = FPoint(-50.0f, -50.0f);
			_timeToShowDucks = INT_MAX;
		}
		else {
			_timeToShowDucks -= dt;
		}

		if (_bigDuckLeft != nullptr && _bigDuckRight != nullptr) {
			_time += dt;
			float scaleFactor = _duckScaleFactor.getGlobalFrame(_time);
			_bigDuckLeft->SetScale(FPoint(scaleFactor, scaleFactor));
			_bigDuckRight->SetScale(FPoint(scaleFactor, scaleFactor));
			_bigDuckLeft->SetPosition(GetPosition() + FPoint(-40 * scaleFactor, 250.0f) + _duckOffset);
			_bigDuckRight->SetPosition(GetPosition() + FPoint(40 * scaleFactor, 250.0f) + _duckOffset);
		}
		// else { noop }
	}

#ifdef _DEBUG
	void IntroAnimator::MouseDown(const IPoint & mouse_pos)
	{
		Message message(_name, "completed");
		Core::messageManager.putMessage(message);
	}
#endif // _DEBUG

	void IntroAnimator::Start()
	{
		_time = 0.0f;
		_duckOffset = FPoint(0.0f, 0.0f);
		_bigDuckLeft = nullptr;
		_bigDuckRight = nullptr;
		ShowCenterText("Triadium & JekaArt", _timeToShowCaptions * 0.8f, -1);
		ShowCenterText("PRESENT", _timeToShowCaptions * 0.6f, 0);
	}

	void IntroAnimator::Init()
	{
		_duckScaleFactor.addKey(0.0f, 0.3f);
		_duckScaleFactor.addKey(4.99f, 0.3f);
		_duckScaleFactor.addKey(5.0f, 0.6f);
		_duckScaleFactor.addKey(8.99f, 0.6f);
		_duckScaleFactor.addKey(9.0f, 1.0f);

		_timeToShowCaptions = 6.0f;
		_timeToShowDucks = 13.3f + _timeToShowCaptions;
		_timeToIntroShow = 12.5f + _timeToShowCaptions;
		_timeToComplete = 15.5f + _timeToShowCaptions;
	}

	void IntroAnimator::ShowInfoText(std::string text, float showTime)
	{
		auto height = _scene->getHeight();
		auto width = _scene->getWidth();
		StageInfoText::Ptr info = new StageInfoText("infotext", "bellflower");
		info->SetScale(0.4f, 0.4f);
		StageInfoText::Model infoModel;
		infoModel.text = text;
		infoModel.startPoint = FPoint(width * -0.5f, height * 0.1f);
		infoModel.endPoint = FPoint(width * 0.5f, height * 0.1f);
		infoModel.time = 0.5f;
		infoModel.showTime = showTime;
		info->SetModel(infoModel);
		_scene->AddObjectLater(info);
	}

	void IntroAnimator::ShowCenterText(std::string text, float showTime, int line)
	{
		auto height = _scene->getHeight();
		auto width = _scene->getWidth();
		StageInfoText::Ptr textObj = new StageInfoText("centertext", "bellflower");
		// info->SetScale(1.0f, 1.0f);
		StageInfoText::Model textModel;
		float hOffset = line * 100;
		float hCoeff = line >= 0 ? -0.5f : 1.5f;
		textModel.text = text;
		textModel.startPoint = FPoint(width * 0.5f, height * hCoeff - hOffset);
		textModel.endPoint = FPoint(width * 0.5f, height * 0.5f - hOffset);
		textModel.time = 0.3f;
		textModel.showTime = showTime;
		textObj->SetModel(textModel);
		_scene->AddObjectLater(textObj);
	}
}
