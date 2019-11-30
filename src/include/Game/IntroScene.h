// Copyright (C) 2019 Aleksey Khachaturov
// https://github.com/triadium/MDHD
// License: GNU GPL 3 or later, see file LICENSE

#pragma once

namespace Game {
	
	// Интро сцена
	class IntroScene : public Scene
	{
	public:

		IntroScene(const std::string& name, rapidxml::xml_node<>* elem);

		void AcceptMessage(const Message& message) override;

	private:
		void Init();
		void AddAnimators();
		void Start();

	private:		
		GUI::WidgetPtr _startButton;
	};

}
