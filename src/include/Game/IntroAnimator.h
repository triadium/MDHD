// Copyright (C) 2019 Aleksey Khachaturov
// https://github.com/triadium/MDHD
// License: GNU GPL 3 or later, see file LICENSE

#pragma once

namespace Game {

	class IntroAnimator: public Object
	{
	public:		
		typedef boost::intrusive_ptr<IntroAnimator> Ptr;
		
		IntroAnimator(const std::string& name);

		void Update(float dt) override;
#ifdef _DEBUG
		void MouseDown(const IPoint &mouse_pos) override;
#endif // _DEBUG
		void Start();

	private:	
		void Init();
		void ShowInfoText(std::string text, float showTime);

	private:
		float _timeToIntroShow;
		float _timeToShowDucks;
		float _timeToComplete;
		ParticleEffectPtr _intro;
		// Подключить бы Spine анимацию и через процессор 
		// обрабатывать это все универсальным способом (функторами с контекстом)
		TimedSpline<float> _duckScaleFactor;
		float _time;
		Object::Ptr _bigDuckLeft;
		Object::Ptr _bigDuckRight;
		FPoint _duckOffset;
	};

}
