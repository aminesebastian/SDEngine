#pragma once
#include "Display.h"
#include "World.h"
#include "Camera.h"
#include "RenderingEngine.h"
#include "Material.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define WINDOW_BIT_DEPTH 8

#define LOG_GENERAL "GENERAL"
#define LOG_WARNING "WARNING"
#define LOG_ERROR "ERROR"

#define PrintToLog(...) _slog(__FILE__,__LINE__,__VA_ARGS__)
#define PrintToLog(...) _slog(__FILE__,__LINE__,LOG_GENERAL,__VA_ARGS__)

struct FKeyInfo {
	bool bWDown;
	bool bADown;
	bool bSDown;
	bool bDDown;
	bool bQDown;
	bool bEDown;
};

class Engine {

public:
	Display* GetDisplay() { return S_Display; }
	URenderingEngine* GetRenderingEngine() { return S_RenderingEngine; }
	UWorld* GetWorld() { return S_World; }
	Camera* GetCurrentCamera() { return S_Camera; }
	bool Init();
	void StartEngine() { bShouldLoop = true; MainLoop(); }
	Material* GetDefaultMaterial() { return S_DefaultMaterial; }

	static Engine* GetInstance();
private:
	Engine();
	virtual ~Engine();
	static Engine* S_EngineInstance;

	Material* S_DefaultMaterial;
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
	FKeyInfo keyInfo;

	Display* S_Display;
	URenderingEngine* S_RenderingEngine;
	UWorld* S_World;
	Camera* S_Camera;
	TwBar* S_InfoBar;

	void MainLoop();
	void GameLoop();
	void RenderingLoop();
	void UILoop();
	void Movement();
};
