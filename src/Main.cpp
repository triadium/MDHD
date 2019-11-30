#include "stdafx.h"
#include "GameAppDelegate.h"

#define GAME_APPLICATION_NAME L"Mega Duck Hunting Deluxe"

#if defined(ENGINE_TARGET_WIN32)
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, const char* argv[])
#endif
{
	ParticleSystem::SetTexturesPath("textures");

#if defined(ENGINE_TARGET_WIN32)
	Core::fileSystem.SetWriteDirectory("./write_directory");
#else
    Core::fileSystem.SetWriteDirectory(IO::Path::GetSpecialFolderPath(SpecialFolder::LocalDocuments));
#endif
    
#if defined(ENGINE_TARGET_WIN32)
    std::string base_path = "base_p";
#else
    std::string base_dir = "Contents/Resources/base_p";
    std::string work_dir = std::string([[[NSBundle mainBundle] bundlePath] UTF8String]);
    std::string base_path = IO::Path::Combine(work_dir, base_dir);
#endif
    
    Core::fileSystem.MountDirectory(base_path);

    Log::log.AddSink(new Log::DebugOutputLogSink());
	Log::log.AddSink(new Log::HtmlFileLogSink("log.htm", true));

#if defined(ENGINE_TARGET_WIN32)
	Core::Application::APPLICATION_NAME = GAME_APPLICATION_NAME;
	Core::RunApplicationWithDelegate(new GameAppDelegate());
#else
	Core::RunApplicationWithDelegate(argc, argv, new GameAppDelegate());
#endif
	
	return 0;
}
