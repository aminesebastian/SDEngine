#pragma once
#include "Core/Engine/Display.h"
#include "Core/Engine/World.h"
#include "Core/Rendering/RenderViewport.h"
#include "Core/Objects/CoreTypes/Material.h"
#include "Core/Objects/CoreTypes/Texture2D.h"
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Input/InputUtilities.h"
#include "Core/Input/IUserInputReciever.h"


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

class Engine : public IUserInputReciever {

public:
	static Engine* GetInstance();
	bool Initialize();
	void StartEngine();

	Display* GetDisplay();
	RenderViewport* GetFocusedViewport();
	World* GetWorld();
	Camera* GetCurrentCamera();
	InputSubsystem* GetInputSubsystem();
	AssetManager* GetAssetManager();
	PictorumRenderer* GetEngineUI();

	bool LoadScene(Scene* SceneToLoad);
	bool RegisterInputReciever(IUserInputReciever* Reciever);
	bool DeregisterInputReciever(IUserInputReciever* Reciever);

	float GetFrameTime();
	float GetFrameRate();
	float GetWorldTime();
	bool IsInGameMode();
	Entity* GetSelectedEntity();
	void SetSelectedEntity(Entity* Entity);
private:
	AssetManager* _AssetManager;
	InputSubsystem* _InputSubsystem;
	EngineUI* _IMGuiEngineUI;
	RenderViewport* _CurrentViewport;
	Display* _Display;
	World* _World;
	Camera* _Camera;
	Scene* _LoadedScene;
	PictorumRenderer* _EngineUI;
	Entity* _SelectedEntity;
	
	float movementSpeed;
	float lookSpeed;

	float DeltaTime;
	float FrameRate;
	float WorldTime;
	Uint64 LastFrameTimecode;
	bool bIsInitialized;
	bool bShouldLoop;
	bool bGameMode;

	Engine();
	virtual ~Engine();
	static Engine* Instance;

	void MainLoop();
	void GameLoop();
	void RenderingLoop();
	void UILoop();
	void InputLoop();

	virtual void OnKeyDown(SDL_Scancode KeyCode) override;
	virtual void OnKeyUp(SDL_Scancode KeyCode) override;
	virtual void OnKeyHeld(SDL_Scancode KeyCode, float HeldTime) override;

	virtual void OnMouseButtonDown(vec2 ScreenPosition, EMouseButton Button) override;
	virtual void OnMouseButtonUp(vec2 ScreenPosition, EMouseButton Button) override;
	virtual void OnMouseAxis(vec2 ScreenPosition, vec2 Delta) override;
	virtual void OnMouseScrollAxis(float Delta) override;
};
