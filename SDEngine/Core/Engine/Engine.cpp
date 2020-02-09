#include "Engine.h"
#include "Core/Assets/AssetManager.h"
#include "Core/Input/InputSubsystem.h"
#include "Core/Objects/CoreTypes/Scene.h"
#include "Core/Objects/Entities/Camera.h"
#include "Core/Objects/Entities/Entity.h"
#include "Core/Objects/Entities/Light.h"
#include "Core/Pictorum/PictorumRenderer.h"
#include "Core/Utilities/Math/MathLibrary.h"
#include "Core/Engine/Windows/MainEditorWindow.h"
#include "Core/Utilities/Logger.h"
#include "Core/Engine/Window.h"
#include "UserInterface/EngineUI.h"


Engine::Engine() {
	_AssetManager              = new AssetManager();
	_InputSubsystem            = new InputSubsystem();
	bGameMode                  = true;
	bIsInitialized             = false;
	bShouldLoop                = false;
	_IMGuiEngineUI             = new EngineUI();

	_OpenWindow = new MainEditorWindow(vec2(1920, 1200));
}
Engine::~Engine() {
	delete _OpenWindow;
	delete _AssetManager;
	delete _InputSubsystem;
}

Engine* Engine::GetInstance() {
	static Engine* S_EngineInstance = new Engine(); // only initialized once!
	return S_EngineInstance;
}

bool Engine::Initialize() {
	if (bIsInitialized) { return false; }
	SD_ENGINE_INFO("Initializing Engine")
	
	_InputSubsystem->Initialize();
	_OpenWindow->Initialize();
	_InputSubsystem->RegisterInputReciever(this);
	_InputSubsystem->RegisterInputReciever(_OpenWindow);

	bIsInitialized = true;
	return true;
}

void Engine::StartEngine() {
	bShouldLoop = true;
	MainLoop();
}

void Engine::MainLoop() {
	while (!_OpenWindow->IsClosed()) {
		InputLoop();
		_OpenWindow->Update();
	}
	//_IMGuiEngineUI->UpdateUI(_MainWindow->GetWindow());
	//_IMGuiEngineUI->RenderUI((float)DeltaTime);
}
void Engine::InputLoop() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		_InputSubsystem->ProcessInputEvent(e);
		switch (e.type) {
			case SDL_QUIT:
				_OpenWindow->Close();
				break;
			case SDL_WINDOWEVENT:
				if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED || e.window.event == SDL_WINDOWEVENT_RESIZED) {
					if (e.window.data1 != _OpenWindow->GetDimensions().x || e.window.data2 != _OpenWindow->GetDimensions().y) {
						_OpenWindow->WindowResized(e.window.data1, e.window.data2);
					}
				}
				break;
			default:
				break;
		}
	}
	_InputSubsystem->Tick(_OpenWindow->GetFrameTime());
}
void Engine::OnKeyDown(SDL_Scancode KeyCode) {
	if (KeyCode == SDL_SCANCODE_RETURN) {
		//_OpenWindow->RecompileShaders();
	}
}
AssetManager* Engine::GetAssetManager() {
	return _AssetManager;
}
InputSubsystem* Engine::GetInputSubsystem() {
	return _InputSubsystem;
}
