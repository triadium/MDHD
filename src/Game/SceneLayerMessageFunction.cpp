#include "stdafx.h"
#include "Game/Constants.h"
#include "Game/Scene.h"
#include "Game/SceneLayerMessageFunction.h"


namespace Game {

	// FIXME: Распространяем каждое сообщение между всеми виджетами указанного слоя в сообщении или текущего
	// Фактически сцену можно превратить в контроллер, который будет манипулировать состоянием сцены,
	// а все остальные будут только посылать сообщения - как виджеты, так и игровые объекты.
	// Недостаток такого подхода состоит в том, что вся логика сосредотачивается внутри одного виджета, 
	// который должен знать как работать со всеми типами игровых объектов, либо опять же осуществлять 
	// широковещательное сообщение всем игровым объектам, либо пропускать - "не мое -  не обрабатываю". 
	// При большом количестве игровых объектов лучше сделать оповещение только тех объектов, 
	// которые подписались на указанное сообщение, но для этого нужно добавить методы объектам - 
	// Размещение на сцене, Удаление со сцены. Сцена будет вызывать их в соответствующих методах.

	void SceneLayerMessageFunction::AcceptMessage(const Message & message)
	{
		const std::string& publisher = message.getPublisher();
		const std::string& data = message.getData();
		auto layer = message.GetTargetLayer() == "" ? Core::mainScreen.GetTopLayer() : Core::guiManager.getLayer(message.GetTargetLayer());		
		layer->BroadcastMessage(message);
	}

}
