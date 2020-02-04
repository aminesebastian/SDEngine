#include "Core/Assets/AssetManager.h"
#include "Core/Input/InputSubsystem.h"
#include "Core/Math/MathLibrary.h"
#include "Core/Pictorum/PictorumRenderer.h"

#include "Engine.h"
#include "Scene.h"
#include "Entities/Entity.h"
#include "Entities/Camera.h"
#include "Entities/Light.h"
#include "Utilities/Logger.h"
#include "UserInterface/EngineUI.h"


Engine::Engine() {
	_Display = new Display(WINDOW_WIDTH, WINDOW_HEIGHT, "SD_Engine", WINDOW_BIT_DEPTH);
	_World = new World();

	Transform cameraTransform;
	cameraTransform.SetRotation(0, glm::radians(50.0f), glm::radians(-180.0f));
	cameraTransform.GetLocation().x = 35;
	cameraTransform.GetLocation().z = 35;
	_Camera = new Camera("Camera", cameraTransform, radians(50.0f), _Display->GetDimensions(), 0.01f, 100000.0f);
	_World->RegisterActor(_Camera);

	_AssetManager = new AssetManager();
	_InputSubsystem = new InputSubsystem();

	bGameMode                = true;
	_LoadedScene           = nullptr;
	_SelectedEntity           = nullptr;
	DeltaTime              = 0.0f;
	FrameRate              = 0;
	WorldTime              = 0.0f;
	LastFrameTimecode          = 0;
	bIsInitialized           = false;
	bShouldLoop              = false;
	movementSpeed            = 300.0f;
	lookSpeed                = 200.0f;

	_CurrentViewport = new RenderViewport(_Display->GetDimensions());
	_IMGuiEngineUI = new EngineUI();
	_EngineUI =  new PictorumRenderer("EditorViewport", _Display->GetDimensions());
}
Engine::~Engine() {
	delete& _Display;
	delete& _CurrentViewport;
}

Engine* Engine::GetInstance() {
	static Engine* S_EngineInstance = new Engine(); // only initialized once!
	return S_EngineInstance;
}

bool Engine::Initialize() {
	if (bIsInitialized) { return false; }
	SD_ENGINE_INFO("Initializing Engine")

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	LastFrameTimecode = SDL_GetPerformanceCounter();

	_CurrentViewport->Initialize();
	_InputSubsystem->Initialize();
	RegisterInputReciever(this);
	RegisterInputReciever(_EngineUI);
	_IMGuiEngineUI->InitalizeUI(_Display->GetWindow(), _Display->GetContext());

	bIsInitialized = true;
	return true;
}
bool Engine::LoadScene(Scene* SceneToLoad) {
	if (_LoadedScene != nullptr) {
		_LoadedScene->SaveScene();
		delete _LoadedScene;
	}

	if (SceneToLoad->LoadScene()) {
		_LoadedScene = SceneToLoad;
		return true;
	} else {
		delete SceneToLoad;
		return false;
	}
}

void Engine::StartEngine() {
	bShouldLoop = true;
	MainLoop();
}

void Engine::MainLoop() {
	while (!_Display->IsClosed()) {
		Uint64 first = SDL_GetPerformanceCounter();

		InputLoop();
		GameLoop();
		RenderingLoop();
		UILoop();

		DeltaTime = ((float)(first - LastFrameTimecode)) / SDL_GetPerformanceFrequency();
		FrameRate = 1.0f / DeltaTime;
		LastFrameTimecode = first;
		WorldTime += DeltaTime;
		_Display->Update();
	}
}
void Engine::GameLoop() {
	_World->TickWorld((float)DeltaTime);
	_IMGuiEngineUI->UpdateUI(_Display->GetWindow());
	_EngineUI->Tick(DeltaTime);
}
void Engine::RenderingLoop() {
	_CurrentViewport->RenderWorld(_World, _Camera);
}
void Engine::UILoop() {
	_EngineUI->Draw(DeltaTime);
	_IMGuiEngineUI->RenderUI((float)DeltaTime);
}
void Engine::InputLoop() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		_InputSubsystem->ProcessInputEvent(e);
		switch (e.type) {
			case SDL_QUIT:
				_Display->CloseDisplay();
				break;
			case SDL_WINDOWEVENT:
				switch (e.window.event) {
					case SDL_WINDOWEVENT_SIZE_CHANGED:
						_Display->WindowResized(e.window.data1, e.window.data2);
						_Camera->SetRenderTargetDimensions(_Display->GetDimensions());
						_EngineUI->OnRenderTargetResolutionChanged(_Display->GetDimensions());
						break;
					case SDL_WINDOWEVENT_RESIZED:
						_Display->WindowResized(e.window.data1, e.window.data2);
						_Camera->SetRenderTargetDimensions(_Display->GetDimensions());
						_EngineUI->OnRenderTargetResolutionChanged(_Display->GetDimensions());
						break;
				}
				break;
			default:
				break;
		}
	}
	_InputSubsystem->Tick(DeltaTime);
}

bool Engine::RegisterInputReciever(IUserInputReciever* Reciever) {
	return _InputSubsystem->RegisterInputReciever(Reciever);
}
bool Engine::DeregisterInputReciever(IUserInputReciever* Reciever) {
	return _InputSubsystem->DeregisterInputReciever(Reciever);
}

void Engine::OnKeyDown(SDL_Scancode KeyCode) {
	if (KeyCode == SDL_SCANCODE_RETURN) {
		_CurrentViewport->RecompileShaders();
	}
	if (KeyCode == SDL_SCANCODE_G) {
		bGameMode = !bGameMode;
	}
}
void Engine::OnKeyUp(SDL_Scancode KeyCode) {

}
void Engine::OnKeyHeld(SDL_Scancode KeyCode, float HeldTime) {
	float alpha = movementSpeed * (float)GetFrameTime();
	if (KeyCode == SDL_SCANCODE_W) {
		_Camera->AddLocation(_Camera->GetTransform().GetForwardVector() * alpha);
	}
	if (KeyCode == SDL_SCANCODE_S) {
		_Camera->AddLocation(-_Camera->GetTransform().GetForwardVector() * alpha);
	}
	if (KeyCode == SDL_SCANCODE_A) {
		_Camera->AddLocation(_Camera->GetTransform().GetRightVector() * alpha);
	}
	if (KeyCode == SDL_SCANCODE_D) {
		_Camera->AddLocation(-_Camera->GetTransform().GetRightVector() * alpha);
	}
	if (KeyCode == SDL_SCANCODE_Q) {
		_Camera->AddLocation(-_Camera->GetTransform().GetUpVector() * alpha);
	}
	if (KeyCode == SDL_SCANCODE_E) {
		_Camera->AddLocation(_Camera->GetTransform().GetUpVector() * alpha);
	}
}

void Engine::OnMouseButtonDown(vec2 ScreenPosition, EMouseButton Button) {
	if (Button == EMouseButton::LEFT) {
		FHitInfo hitInfo;
		if (MathLibrary::LineTraceAgainstWorldFromScreen(hitInfo, vec2(ScreenPosition.x, ScreenPosition.y), _Camera, _World, EDITOR)) {
			SetSelectedEntity(hitInfo.HitEntity);
		} else {
			SetSelectedEntity(nullptr);
		}
	}
}
void Engine::OnMouseButtonUp(vec2 ScreenPosition, EMouseButton Button) {

}
void Engine::OnMouseAxis(vec2 ScreenPosition, vec2 Delta) {
	GetFocusedViewport()->OnMouseMove(ScreenPosition);
	if (_InputSubsystem->IsMouseButtonDown(EMouseButton::RIGHT)) {
		SDL_ShowCursor(0);
		_Camera->AddOrbit((float)(-Delta.y) / lookSpeed, -(float)(Delta.x) / lookSpeed);
	} else if (_InputSubsystem->IsMouseButtonDown(EMouseButton::MIDDLE)) {
		SDL_ShowCursor(0);
		_Camera->AddLocation(-_Camera->GetTransform().GetRightVector() * (float)(Delta.x) / 250.0f);
		_Camera->AddLocation(_Camera->GetTransform().GetUpVector() * (float)(Delta.y) / 250.0f);
	} else {
		SDL_ShowCursor(1);
	}
}
void Engine::OnMouseScrollAxis(float Delta) {
	movementSpeed = glm::clamp(movementSpeed + ((float)Delta * GetFrameTime() * 1000.0f), 1.0f, 1000.0f);
}

AssetManager* Engine::GetAssetManager() {
	return _AssetManager;
}
Display* Engine::GetDisplay() {
	return _Display;
}
RenderViewport* Engine::GetFocusedViewport() {
	return _CurrentViewport;
}
World* Engine::GetWorld() {
	return _World;
}
Camera* Engine::GetCurrentCamera() {
	return _Camera;
}
InputSubsystem* Engine::GetInputSubsystem() {
	return _InputSubsystem;
}
PictorumRenderer* Engine::GetEngineUI() {
	return _EngineUI;
}
float Engine::GetFrameTime() {
	return DeltaTime;
}
float Engine::GetFrameRate() {
	return FrameRate;
}
float Engine::GetWorldTime() {
	return WorldTime;
}
bool Engine::IsInGameMode() {
	return bGameMode;
}
Entity* Engine::GetSelectedEntity() {
	return _SelectedEntity;
}
void Engine::SetSelectedEntity(Entity* Entity) {
	_SelectedEntity = Entity;
}
