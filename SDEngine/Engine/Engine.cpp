#include "AssetManager.h"
#include "Engine.h"
#include "Scene.h"
#include "Entities/Entity.h"
#include "Entities/Camera.h"
#include "Entities/Light.h"
#include "Utilities/Logger.h"
#include "UserInterface/EngineUI.h"
#include "Core/Math/MathLibrary.h"

#include "Lib/Imgui/imgui.h"
#include "Lib/Imgui/imgui_impl_sdl.h"
#include "Lib/Imgui/imgui_impl_opengl3.h"

Engine::Engine() {
	S_Display = new Display(WINDOW_WIDTH, WINDOW_HEIGHT, "SD_Engine", WINDOW_BIT_DEPTH);
	S_World = new World();

	Transform cameraTransform;
	cameraTransform.SetRotation(0, glm::radians(50.0f), glm::radians(-180.0f));
	cameraTransform.GetLocation().x = 35;
	cameraTransform.GetLocation().z = 35;
	S_Camera = new Camera("Camera", cameraTransform, radians(50.0f), S_Display->GetDimensions(), 0.01f, 100000.0f);
	S_World->RegisterActor(S_Camera);

	S_AssetManager = new AssetManager();

	for (int i = 0; i < 322; i++) {
		S_InputKeys[i].bKeyDown = false;
	}

	bGameMode       = true;
	S_CurrentScene  = nullptr;
	SelectedEntity  = nullptr;
	S_DeltaTime     = 0.0f;
	S_FrameRate     = 0;
	S_WorldTime     = 0.0f;
	S_LastFrameTime = 0;
	bIsInitialized  = false;
	bShouldLoop     = false;
	lastMouseX      = 0;
	lastMouseY      = 0;
	movementSpeed   = 300.0f;
	lookSpeed       = 200.0f;
	MousePosition   = vec2(0.0f, 0.0f);

	FocusedViewport = new RenderViewport(S_Display->GetDimensions());
	S_EngineUI = new EngineUI();
}
Engine::~Engine() {
	delete& S_Display;
	delete& FocusedViewport;
}

Engine* Engine::GetInstance() {
	static Engine* S_EngineInstance = new Engine(); // only initialized once!
	return S_EngineInstance;
}

bool Engine::Initialize() {
	if (bIsInitialized) { return false; }
	SD_ENGINE_INFO("Initializing Engine")

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	S_LastFrameTime = SDL_GetPerformanceCounter();

	FocusedViewport->Initialize();
	S_EngineUI->InitalizeUI(S_Display->GetWindow(), S_Display->GetContext());

	bIsInitialized = true;
	return true;
}
bool Engine::LoadScene(Scene* SceneToLoad) {
	if (S_CurrentScene != nullptr) {
		S_CurrentScene->SaveScene();
		delete S_CurrentScene;
	}

	if (SceneToLoad->LoadScene()) {
		S_CurrentScene = SceneToLoad;
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
	while (!S_Display->IsClosed()) {
		Uint64 first = SDL_GetPerformanceCounter();

		InputLoop();
		GameLoop();
		RenderingLoop();
		UILoop();

		S_DeltaTime = ((float)(first - S_LastFrameTime)) / SDL_GetPerformanceFrequency();
		S_FrameRate = 1.0f / S_DeltaTime;
		S_LastFrameTime = first;
		S_WorldTime += S_DeltaTime;
		S_Display->Update();
	}
}
void Engine::GameLoop() {
	S_World->TickWorld((float)S_DeltaTime);
	S_EngineUI->UpdateUI(S_Display->GetWindow());
}
void Engine::RenderingLoop() {
	FocusedViewport->RenderWorld(S_World, S_Camera);
}
void Engine::UILoop() {
	S_EngineUI->RenderUI((float)S_DeltaTime);
}
void Engine::InputLoop() {
	SDL_Event e;

	while (SDL_PollEvent(&e)) {
		ImGuiIO& io = ImGui::GetIO();
		if (io.WantCaptureMouse || io.WantCaptureKeyboard) {
			ImGui_ImplSDL2_ProcessEvent(&e);
			return;
		}

		switch (e.type) {
			case SDL_QUIT:
				S_Display->CloseDisplay();
				break;
			case SDL_KEYDOWN:
				S_InputKeys[e.key.keysym.scancode].bKeyDown = true;
				OnKeyDown(e.key.keysym.scancode);
				break;
			case SDL_KEYUP:
				S_InputKeys[e.key.keysym.scancode].bKeyDown = false;
				OnKeyUp(e.key.keysym.scancode);
				break;
			case SDL_WINDOWEVENT:
				switch (e.window.event) {
					case SDL_WINDOWEVENT_SIZE_CHANGED:
						S_Display->WindowResized(e.window.data1, e.window.data2);
						S_Camera->SetRenderTargetDimensions(S_Display->GetDimensions());
						break;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (e.button.button == SDL_BUTTON_LMASK) {
					FHitInfo hitInfo;
					if (MathLibrary::LineTraceAgainstWorldFromScreen(hitInfo, vec2(MousePosition.x, MousePosition.y), S_Camera, S_World, EDITOR)) {
						SetSelectedEntity(hitInfo.HitEntity);
					} else {
						SetSelectedEntity(nullptr);
					}			
				}
				break;
			case SDL_MOUSEMOTION:
				MousePosition = vec2(e.motion.x, e.motion.y);
				if (e.type == SDL_MOUSEMOTION) {
					GetFocusedViewport()->OnMouseMove(MousePosition);
					if (e.motion.state & SDL_BUTTON_RMASK) {
						SDL_ShowCursor(0);
						S_Camera->AddOrbit((float)(e.motion.y - lastMouseY) / lookSpeed, -(float)(e.motion.x - lastMouseX) / lookSpeed);
						lastMouseX = e.motion.x;
						lastMouseY = e.motion.y;
					} else if (e.motion.state & SDL_BUTTON_MMASK) {
						SDL_ShowCursor(0);
						S_Camera->AddLocation(-S_Camera->GetTransform().GetRightVector() * (float)(e.motion.x - lastMouseX) / 250.0f);
						S_Camera->AddLocation(S_Camera->GetTransform().GetUpVector() * (float)(e.motion.y - lastMouseY) / 250.0f);
						lastMouseX = e.motion.x;
						lastMouseY = e.motion.y;
					} else {
						SDL_ShowCursor(1);
						lastMouseX = e.motion.x;
						lastMouseY = e.motion.y;
					}
				}
				break;
			case SDL_MOUSEWHEEL:
				movementSpeed = glm::clamp(movementSpeed + ((float)e.wheel.y * GetFrameTime() * 1000.0f), 1.0f, 1000.0f);
				break;
			default:
				break;
		}
	}
	KeyAxisMapping();
}

void Engine::OnKeyDown(int KeyCode) {
	if (KeyCode == SDL_SCANCODE_RETURN) {
		FocusedViewport->RecompileShaders();
	}
	//if (KeyCode == SDL_SCANCODE_F) {
	//	S_Camera->SetTransform(S_Camera->GetInitialTransform());
	//}
	if (S_InputKeys[SDL_SCANCODE_P].bKeyDown) {
		FocusedViewport->SetDebugEnabled(!FocusedViewport->GetDebugEnabled());
	}
	if (S_InputKeys[SDL_SCANCODE_G].bKeyDown) {
		bGameMode = !bGameMode;
	}
	if (S_InputKeys[SDL_SCANCODE_1].bKeyDown) {
		if (FocusedViewport->GetDebugEnabled()) {
			FocusedViewport->SetDebugState(WIREFRAME);
		}
	}
	if (S_InputKeys[SDL_SCANCODE_2].bKeyDown) {
		if (FocusedViewport->GetDebugEnabled()) {
			FocusedViewport->SetDebugState(ALBEDO);
		}
	}
	if (S_InputKeys[SDL_SCANCODE_3].bKeyDown) {
		if (FocusedViewport->GetDebugEnabled()) {
			FocusedViewport->SetDebugState(NORMAL);
		}
	}
	if (S_InputKeys[SDL_SCANCODE_4].bKeyDown) {
		if (FocusedViewport->GetDebugEnabled()) {
			FocusedViewport->SetDebugState(WORLD_POSITION);
		}
	}
	if (S_InputKeys[SDL_SCANCODE_5].bKeyDown) {
		if (FocusedViewport->GetDebugEnabled()) {
			FocusedViewport->SetDebugState(DETAIL_LIGHT);
		}
	}
}
void Engine::OnKeyUp(int KeyCode) {

}
void Engine::KeyAxisMapping() {
	if (S_InputKeys[SDL_SCANCODE_W].bKeyDown) {
		S_Camera->AddLocation(S_Camera->GetTransform().GetForwardVector() * movementSpeed * (float)GetFrameTime());
	}
	if (S_InputKeys[SDL_SCANCODE_S].bKeyDown) {
		S_Camera->AddLocation(-S_Camera->GetTransform().GetForwardVector() * movementSpeed * (float)GetFrameTime());
	}
	if (S_InputKeys[SDL_SCANCODE_A].bKeyDown) {
		S_Camera->AddLocation(S_Camera->GetTransform().GetRightVector() * movementSpeed * (float)GetFrameTime());
	}
	if (S_InputKeys[SDL_SCANCODE_D].bKeyDown) {
		S_Camera->AddLocation(-S_Camera->GetTransform().GetRightVector() * movementSpeed * (float)GetFrameTime());
	}
	if (S_InputKeys[SDL_SCANCODE_Q].bKeyDown) {
		S_Camera->AddLocation(-S_Camera->GetTransform().GetUpVector() * movementSpeed * (float)GetFrameTime());
	}
	if (S_InputKeys[SDL_SCANCODE_E].bKeyDown) {
		S_Camera->AddLocation(S_Camera->GetTransform().GetUpVector() * movementSpeed * (float)GetFrameTime());
	}
}

AssetManager* Engine::GetAssetManager() {
	return S_AssetManager;
}
Display* Engine::GetDisplay() {
	return S_Display;
}
RenderViewport* Engine::GetFocusedViewport() {
	return FocusedViewport;
}
World* Engine::GetWorld() {
	return S_World;
}
Camera* Engine::GetCurrentCamera() {
	return S_Camera;
}
vec2 Engine::GetScreenRes() {
	return vec2(S_Display->GetDimensions().x, S_Display->GetDimensions().y);
}
float Engine::GetFrameTime() {
	return S_DeltaTime;
}
float Engine::GetFrameRate() {
	return S_FrameRate;
}
float Engine::GetWorldTime() {
	return S_WorldTime;
}
bool Engine::IsInGameMode() {
	return bGameMode;
}
Entity* Engine::GetSelectedEntity() {
	return SelectedEntity;
}
void Engine::SetSelectedEntity(Entity* Entity) {
	SelectedEntity = Entity;
}
vec2 Engine::GetMousePosition() {
	return MousePosition;
}
