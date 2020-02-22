#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Input/InputUtilities.h"
#include "Core/Input/IUserInputReciever.h"
#include "Core/Engine/Delegates/Event.h"


#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1200
#define WINDOW_BIT_DEPTH 8

#define LOG_GENERAL "GENERAL"
#define LOG_WARNING "WARNING"
#define LOG_ERROR "ERROR"

#ifndef MacrosDefined
#define MacrosDefined
#define PrintToLog(...) _slog(__FILE__,__LINE__,LOG_GENERAL,__VA_ARGS__)
#endif

class AssetManager;
class InputSubsystem;
class Camera;
class Scene;
class EngineUI;
class PictorumRenderer;
class Window;

class Engine : public IUserInputReciever {

public:
	static Engine* Get();
	static AssetManager* GetAssetManager();
	static InputSubsystem* GetInputSubsystem();

	bool Initialize();
	void StartEngine();

	Event<void(void)> RecompileShaders;
private:
	AssetManager* _AssetManager;
	InputSubsystem* _InputSubsystem;
	Window* _OpenWindow;

	bool bIsInitialized;
	bool bShouldLoop;
	bool bGameMode;

	Engine();
	virtual ~Engine();
	static Engine* Instance;

	void MainLoop();
	void InputLoop();

	virtual void OnKeyDown(SDL_Scancode KeyCode) override;
};
