#include "stdafx.h"
#include "Game/Constants.h"
#include "Game/IntroScene.h"
#include "Game/StageScene.h"
#include "Game/SceneLayerMessageFunction.h"
#include "GameAppDelegate.h"

GameAppDelegate::GameAppDelegate()
{
}

void GameAppDelegate::GameContentSize(int deviceWidth, int deviceHeight, int &width, int &height)
{
	width = WINDOW_WIDTH;
	height = WINDOW_HEIGHT;
}

void GameAppDelegate::ScreenMode(DeviceMode &mode)
{
	mode = DeviceMode::Windowed;
}

void GameAppDelegate::RegisterTypes()
{
	REGISTER_WIDGET_XML(Game::IntroScene, Game::Names::IntroScene);
	REGISTER_WIDGET_XML(Game::StageScene, Game::Names::StageScene);
}

void GameAppDelegate::LoadResources()
{
	Core::LuaExecuteStartupScript("start.lua");
	Core::guiManager.getLayer(Game::Names::IntroLayer)->messageFunc = new Game::SceneLayerMessageFunction();
	Core::guiManager.getLayer(Game::Names::StageLayer)->messageFunc = new Game::SceneLayerMessageFunction();
}

void GameAppDelegate::OnResourceLoaded() {
	if (Core::appInstance->AllResourcesLoaded()) {
		Core::appInstance->SetLoaded();
	}
	// else { go on }
}

#ifdef _DEBUG

void GameAppDelegate::OnPostDraw() {
	if (Render::isFontLoaded("arial")) {

		Render::BindFont("arial");

		int dy = Render::getFontHeight();
		int x = Render::device.Width() - 5;
		int y = 100;

		Render::PrintString(x, y, std::string("FPS: ") + utils::lexical_cast(Core::appInstance->GetCurrentFps()), 1.0f, RightAlign, BottomAlign);
		Render::PrintString(x, y -= dy, std::string("Video: ") + utils::lexical_cast(Render::device.GetVideoMemUsage() / 1024) + std::string("K"), 1.0f, RightAlign, BottomAlign);
		Render::PrintString(x, y -= dy, std::string("Audio: ") + utils::lexical_cast(Core::resourceManager.GetMemoryInUse<MM::AudioResource>() / 1024) + std::string("K"), 1.0f, RightAlign, BottomAlign);
		Render::PrintString(x, y -= dy, std::string("Animations: ") + utils::lexical_cast((Core::resourceManager.GetMemoryInUse<Render::StreamingAnimation>() + Core::resourceManager.GetMemoryInUse<Render::Animation>()) / 1024) + std::string("K"), 1.0f, RightAlign, BottomAlign);
		Render::PrintString(x, y -= dy, std::string("Textures: ") + utils::lexical_cast(Core::resourceManager.GetMemoryInUse<Render::Texture>() / 1024) + std::string("K"), 1.0f, RightAlign, BottomAlign);
		Render::PrintString(x, y -= dy, std::string("Particles: ") + utils::lexical_cast(Core::resourceManager.GetMemoryInUse<ParticleEffect>() / 1024) + std::string("K"), 1.0f, RightAlign, BottomAlign);
		Render::PrintString(x, y -= dy, std::string("Models: ") + utils::lexical_cast(Core::resourceManager.GetMemoryInUse<Render::ModelAnimation>() / 1024) + std::string("K"), 1.0f, RightAlign, BottomAlign);
	}
	// else { skip }
}
#endif // _DEBUG


