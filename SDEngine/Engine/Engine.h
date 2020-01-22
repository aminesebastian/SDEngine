#pragma once
#include "Display.h"
#include "Engine/World.h"
#include "RenderViewport.h"
#include "Rendering/Material.h"
#include "Rendering/Texture2D.h"
#include "Utilities/TypeDefenitions.h"

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
class Camera;
class Scene;
class EngineUI;

struct FInputKey {
	bool bKeyDown;
};

class Engine {

public:
	Display* GetDisplay();
	RenderViewport* GetFocusedViewport();
	UWorld* GetWorld();
	Camera* GetCurrentCamera();
	bool Initialize();
	void StartEngine();
	vec2 GetScreenRes();


	float GetFrameTime();
	float GetFrameRate();
	float GetWorldTime();

	static Engine* GetInstance();

	AssetManager* GetAssetManager();

	bool IsInGameMode();

	bool LoadScene(Scene* SceneToLoad);

	Entity* GetSelectedEntity();
	void SetSelectedEntity(Entity* Entity);
	vec2 GetMousePosition();

private:
	Engine();
	virtual ~Engine();
	static Engine* S_EngineInstance;

	Entity* SelectedEntity;

	vec2 MousePosition;

	AssetManager* S_AssetManager;

	Scene* S_CurrentScene;

	EngineUI* S_EngineUI;

	float S_DeltaTime;
	float S_FrameRate;
	float S_WorldTime;
	Uint64 S_LastFrameTime;
	bool bIsInitialized;
	bool bShouldLoop;


	int lastMouseX;
	int lastMouseY;
	float movementSpeed;
	float lookSpeed;

	FInputKey S_InputKeys[1000];

	Display* S_Display;
	RenderViewport* FocusedViewport;
	UWorld* S_World;
	Camera* S_Camera;

	bool bGameMode;

	void MainLoop();
	void GameLoop();
	void RenderingLoop();
	void UILoop();
	void InputLoop();

	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	void KeyAxisMapping();
};
