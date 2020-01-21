#pragma once
#include "Display.h"
#include "World.h"
#include "RenderingEngine.h"
#include "Material.h"
#include "Texture2D.h"
#include "TypeDefenitions.h"

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
	Display* GetDisplay() { return S_Display; }
	URenderingEngine* GetRenderingEngine() { return S_RenderingEngine; }
	UWorld* GetWorld() { return S_World; }
	Camera* GetCurrentCamera() { return S_Camera; }
	bool Init();
	void StartEngine() { bShouldLoop = true; MainLoop(); }
	vec2 GetScreenRes() { return vec2(S_Display->GetDimensions().x, S_Display->GetDimensions().y); }


	float GetFrameTime() { return S_DeltaTime; }
	float GetFrameRate() { return S_FrameRate; }
	float GetWorldTime() { return S_WorldTime; }

	static Engine* GetInstance();

	AssetManager* GetAssetManager();

	bool IsInGameMode() { return bGameMode; }

	bool LoadScene(Scene* SceneToLoad);

	Entity* GetSelectedEntity();
	void SetSelectedEntity(Entity* Entity);

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
	URenderingEngine* S_RenderingEngine;
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
