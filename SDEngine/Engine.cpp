#include "Engine.h"
#include "Light.h"
#include "AssetManager.h"
#include <AntTweakBar.h>
#include "Camera.h"

Engine::Engine() {
	S_Display = new Display(WINDOW_WIDTH, WINDOW_HEIGHT, "SD_Engine", WINDOW_BIT_DEPTH);
	S_RenderingEngine = new URenderingEngine(S_Display);
	S_World = new UWorld();

	Transform cameraTransform;
	cameraTransform.SetRotation(0, 50, -180);
	cameraTransform.GetPosition().x -= 2;
	S_Camera = new Camera(cameraTransform, radians(50.0f), S_Display->GetAspectRatio(), 0.01f, 1000.0f);

	TwInit(TW_OPENGL, NULL);
	TwWindowSize(S_Display->GetDimensions().x, S_Display->GetDimensions().y);
	S_InfoBar = TwNewBar("StatUnit");
	TwDefine(" StatUnit refresh=0.1 ");
	TwDefine(" StatUnit alpha=30 ");
	TwAddVarRO(S_InfoBar, "Frame Time", TW_TYPE_FLOAT, &S_DeltaTime, "");
	TwAddVarRO(S_InfoBar, "Frame Rate", TW_TYPE_INT32, &S_FrameRate, "");

	S_AssetManager = new AssetManager();

	for (int i = 0; i < 322; i++) {
		S_InputKeys[i].bKeyDown = false;
	}
	lightRotateAngle = 0;
	lightRotateRadius = 2;
	lightRotateSpeed = (2 * 3.14159265358979323846264338327950288) / 25000;
}
Engine::~Engine() {
	delete &S_Display;
	delete &S_RenderingEngine;
}

Engine* Engine::GetInstance() {
	static Engine* S_EngineInstance = new Engine(); // only initialized once!
	return S_EngineInstance;
}
AssetManager* Engine::GetAssetManager() {
	return S_AssetManager;
}

bool Engine::Init() {
	if (bIsInitialized) { return false; }

	S_RenderingEngine->ChangeShader("./Res/Shaders/DefaultGeometryPassShader");
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	bIsInitialized = true;
	return true;
}

void Engine::MainLoop()  {
	while (!S_Display->IsClosed()) {
		float now = SDL_GetTicks();

		InputLoop();
		GameLoop();
		RenderingLoop();
		UILoop();

		S_DeltaTime = now - S_LastFrameTime;
		S_FrameRate = 1.0f / (S_DeltaTime/1000.0f);
		S_LastFrameTime = now;
		S_WorldTime += S_DeltaTime;
		S_Display->Update();
	}
}
void Engine::GameLoop() {
	S_World->TickWorld(S_DeltaTime);
}
void Engine::RenderingLoop() {
	for (int i = 0; i < S_World->GetWorldLights().size(); i++) {
		lightRotateAngle += lightRotateSpeed * S_DeltaTime;
		float x = glm::cos(lightRotateAngle+(i*0.23))*lightRotateRadius;
		float y = glm::sin(lightRotateAngle+(i*0.23))*lightRotateRadius;


		S_World->GetWorldLights()[i]->GetTransform().GetPosition().x = x;
		S_World->GetWorldLights()[i]->GetTransform().GetPosition().y = y;
	}

	S_RenderingEngine->RenderWorld(S_World, S_Camera);
}
void Engine::UILoop() {
	TwDraw();
}
void Engine::InputLoop() {
	SDL_Event e;
	bool bHandled;
	while (SDL_PollEvent(&e)) {
		bHandled = TwEventSDL(&e, SDL_MAJOR_VERSION, SDL_MINOR_VERSION);
		if(bHandled) {
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
			case SDL_WINDOWEVENT_RESIZED:
				S_Display->ResizeDisplay(e.window.data1, e.window.data2);
				break;
			case SDL_MOUSEBUTTONDOWN:
				break;
			case SDL_MOUSEMOTION:
				if (e.type == SDL_MOUSEMOTION) {
					if (e.motion.state & SDL_BUTTON_RMASK) {
						SDL_ShowCursor(0);
						S_Camera->AddOrbit((float)(e.motion.y - lastMouseY) / lookSpeed, -(float)(e.motion.x - lastMouseX) / lookSpeed);
						lastMouseX = e.motion.x;
						lastMouseY = e.motion.y;
					}
					else if (e.motion.state & SDL_BUTTON_MMASK) {
						SDL_ShowCursor(0);
						S_Camera->GetTransform().GetPosition() -= S_Camera->GetTransform().GetRightVector()*(float)(e.motion.x - lastMouseX) / 250.0f;
						S_Camera->GetTransform().GetPosition() += S_Camera->GetTransform().GetUpVector()*(float)(e.motion.y - lastMouseY) / 250.0f;
						lastMouseX = e.motion.x;
						lastMouseY = e.motion.y;
					}
					else {
						SDL_ShowCursor(1);
						lastMouseX = e.motion.x;
						lastMouseY = e.motion.y;
					}
				}
				break;
			case SDL_MOUSEWHEEL:
				movementSpeed = clamp(movementSpeed + ((float)e.wheel.y / 100.0f), 0.01f, 2.5f);
				break;
			default:
				break;
		}
	}
	KeyAxisMapping();
}

void Engine::OnKeyDown(int KeyCode) {
	if (KeyCode == SDL_SCANCODE_RETURN) {
		S_RenderingEngine->RecompileShaders(S_World);
	}
	if (KeyCode == SDL_SCANCODE_F) {
		S_Camera->SetTransform(S_Camera->GetInitialTransform());
	}
	if (KeyCode == SDL_SCANCODE_L) {
		for (int i = 0; i < S_World->GetWorldLights().size(); i++) {
			S_World->GetWorldLights()[i]->ToggleDebug(!S_World->GetWorldLights()[i]->GetDebugMode());
		}
	}
	if (S_InputKeys[SDL_SCANCODE_G].bKeyDown) {
		S_RenderingEngine->SetDebugEnabled(!S_RenderingEngine->GetDebugEnabled());
	}
	if (S_InputKeys[SDL_SCANCODE_1].bKeyDown) {
		if(S_RenderingEngine->GetDebugEnabled()) {
			S_RenderingEngine->SetDebugState(WIREFRAME);
		}
	}
	if (S_InputKeys[SDL_SCANCODE_2].bKeyDown) {
		if (S_RenderingEngine->GetDebugEnabled()) {
			S_RenderingEngine->SetDebugState(ALBEDO);
		}
	}
	if (S_InputKeys[SDL_SCANCODE_3].bKeyDown) {
		if (S_RenderingEngine->GetDebugEnabled()) {
			S_RenderingEngine->SetDebugState(NORMAL);
		}
	}
	if (S_InputKeys[SDL_SCANCODE_4].bKeyDown) {
		if (S_RenderingEngine->GetDebugEnabled()) {
			S_RenderingEngine->SetDebugState(WORLD_POSITION);
		}
	}
	if (S_InputKeys[SDL_SCANCODE_5].bKeyDown) {
		if (S_RenderingEngine->GetDebugEnabled()) {
			S_RenderingEngine->SetDebugState(DETAIL_LIGHT);
		}
	}
}
void Engine::OnKeyUp(int KeyCode) {

}
void Engine::KeyAxisMapping() {
	if (S_InputKeys[SDL_SCANCODE_W].bKeyDown) {
		S_Camera->GetTransform().GetPosition() = S_Camera->GetTransform().GetPosition() + S_Camera->GetTransform().GetForwardVector() * movementSpeed;
	}
	if (S_InputKeys[SDL_SCANCODE_S].bKeyDown) {
		S_Camera->GetTransform().GetPosition() = S_Camera->GetTransform().GetPosition() - S_Camera->GetTransform().GetForwardVector() * movementSpeed;
	}
	if (S_InputKeys[SDL_SCANCODE_A].bKeyDown) {
		S_Camera->GetTransform().GetPosition() = S_Camera->GetTransform().GetPosition() + S_Camera->GetTransform().GetRightVector() * movementSpeed;
	}
	if (S_InputKeys[SDL_SCANCODE_D].bKeyDown) {
		S_Camera->GetTransform().GetPosition() = S_Camera->GetTransform().GetPosition() - S_Camera->GetTransform().GetRightVector() * movementSpeed;
	}
	if (S_InputKeys[SDL_SCANCODE_Q].bKeyDown) {
		S_Camera->GetTransform().GetPosition() = S_Camera->GetTransform().GetPosition() - S_Camera->GetTransform().GetUpVector() * movementSpeed;
	}
	if (S_InputKeys[SDL_SCANCODE_E].bKeyDown) {
		S_Camera->GetTransform().GetPosition() = S_Camera->GetTransform().GetPosition() + S_Camera->GetTransform().GetUpVector() * movementSpeed;
	}
}
