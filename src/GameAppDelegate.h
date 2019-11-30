#pragma once

class GameAppDelegate : public Core::EngineAppDelegate {
public:
	GameAppDelegate();

	virtual void GameContentSize(int deviceWidth, int deviceHeight, int &width, int &height) override;
	virtual void ScreenMode(DeviceMode &mode) override;

	virtual void RegisterTypes() override;

	virtual void LoadResources() override;
	virtual void OnResourceLoaded() override;

#ifdef _DEBUG
	virtual void OnPostDraw() override;
#endif // _DEBUG

};
