#include "stdafx.h"

#include "Game/Constants.h"
#include "Game/Scene.h"
#include "Game/Object.h"
#include "Game/Processor.h"
#include "Game/UpdateProcessor.h"

#include "Game/IntroAnimator.h"

#include "Game/IntroScene.h"

namespace Game {

	IntroScene::IntroScene(const std::string & name, rapidxml::xml_node<>* elem)
		: Scene(name, elem)
	{
		Init();
	}

	void IntroScene::AcceptMessage(const Message & message)
	{
		const std::string& publisher = message.getPublisher();
		const std::string& data = message.getData();
		if (publisher == "Layer") {
			if (data == "LayerInit") {
				_startButton = getChild("StartButton");
				_startButton->setVisible(false);
				AddAnimators();
			}
			// else { skip }
		}
		else if (publisher == "StartButton" && data == "press") {
			Start();
		}
		else if (publisher == "introanimator" && data == "completed") {
			_startButton->setVisible(true);
		}
		// else { skip }
	}

	void IntroScene::Init()
	{
		AddProcessor(std::make_unique<UpdateProcessor>(this));
	}

	void IntroScene::AddAnimators()
	{
		IntroAnimator::Ptr animator = new IntroAnimator("introanimator");
		animator->SetPosition(0.5f * width, 0.5f * height);
		AddObject(animator);
		animator->Start();
	}

	void IntroScene::Start()
	{
		MM::manager.FadeAll(1.0f);
		Clear();
		Core::mainScreen.popLayer();
		Core::mainScreen.pushLayer(Names::StageLayer);
	}

}