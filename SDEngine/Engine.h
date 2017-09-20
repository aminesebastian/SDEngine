#pragma once
#include "Display.h"
#include "World.h"
#include "RenderingEngine.h"
#include "Material.h"
#include "Texture2D.h"
#include "TypeDefenitions.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_BIT_DEPTH 10

#define LOG_GENERAL "GENERAL"
#define LOG_WARNING "WARNING"
#define LOG_ERROR "ERROR"

#define PrintToLog(...) _slog(__FILE__,__LINE__,__VA_ARGS__)
#define PrintToLog(...) _slog(__FILE__,__LINE__,LOG_GENERAL,__VA_ARGS__)

class AssetManager;
class Camera;

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

	float GetDeltaTime() { return S_DeltaTime; }
	float GetWorldTime() { return S_WorldTime; }

	static Engine* GetInstance();

	AssetManager* GetAssetManager();

	TwBar* GetInfoBar() { return S_InfoBar; }
private:
	Engine();
	virtual ~Engine();
	static Engine* S_EngineInstance;

	AssetManager* S_AssetManager;

	float S_DeltaTime = 0.0f;	
	int S_FrameRate = 0;
	float S_WorldTime = 0.0f;
	long S_LastFrameTime = 0.0f;
	bool bIsInitialized = false;
	bool bShouldLoop = false;
	
	/*Temporary*/
	int lastMouseX = 0;
	int lastMouseY = 0;
	float movementSpeed = 0.5f;
	float lookSpeed = 200.0f;

	FInputKey S_InputKeys[1000];

	Display* S_Display;
	URenderingEngine* S_RenderingEngine;
	UWorld* S_World;
	Camera* S_Camera;
	TwBar* S_InfoBar;

	void MainLoop();
	void GameLoop();
	void RenderingLoop();
	void UILoop();
	void InputLoop();

	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	void KeyAxisMapping();
};
