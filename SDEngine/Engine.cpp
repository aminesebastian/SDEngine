#include "Engine.h"
#include "Light.h"
#include "AssetManager.h"
#include <AntTweakBar.h>
#include "Camera.h"

Engine::Engine() {
	S_Display = new Display(WINDOW_WIDTH, WINDOW_HEIGHT, "SD_Engine", WINDOW_BIT_DEPTH);
	S_RenderingEngine = new URenderingEngine(S_Display);
	S_World = new UWorld();

	Transform cameraTransform(vec3(0, 15, 5));
	cameraTransform.SetRotation(50, -180, 0);
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
	//for (int i = 0; i < S_World->GetWorldLights().size(); i++) {
	//	switch (i % 5) {
	//	case 0:
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().x = (1 * glm::sin((S_WorldTime + ((float)i / 10000.0f)) / 500 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().x;
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().y = (1 * glm::sin((S_WorldTime + ((float)i / 10000.0f)) / 400 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().y;
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().z = (1 * glm::cos((S_WorldTime + ((float)i / 10000.0f)) / 600 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().z;
	//		break;
	//	case 1:
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().x = (-1 * glm::sin((S_WorldTime + ((float)i / 10000.0f)) / 400 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().x;
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().y = (-1 * glm::sin((S_WorldTime + ((float)i / 10000.0f)) / 500 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().y;
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().z = (-1 * glm::cos((S_WorldTime + ((float)i / 10000.0f)) / 600 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().z;
	//		break;
	//	case 2:
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().x = (1 * glm::sin((S_WorldTime + ((float)i / 10000.0f)) / 100 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().x;
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().y = (1 * glm::sin((S_WorldTime + ((float)i / 10000.0f)) / 400 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().y;
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().z = (1 * glm::cos((S_WorldTime + ((float)i / 10000.0f)) / 500 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().z;
	//		break;
	//	case 3:
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().x = (-1 * glm::sin((S_WorldTime + ((float)i / 10000.0f)) / 400 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().x;
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().y = (-1 * glm::sin((S_WorldTime + ((float)i / 10000.0f)) / 600 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().y;
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().z = (-1 * glm::cos((S_WorldTime + ((float)i / 10000.0f)) / 800 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().z;
	//		break;
	//	case 4:
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().x = (1 * glm::sin((S_WorldTime + ((float)i / 10000.0f)) / 500 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().x;
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().y = (1 * glm::sin((S_WorldTime + ((float)i / 10000.0f)) / 100 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().y;
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().z = (1 * glm::cos((S_WorldTime + ((float)i / 10000.0f)) / 600 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().z;
	//		break;
	//	default:
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().x = (1 * glm::sin((S_WorldTime + ((float)i / 10000.0f)) / 500 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().x;
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().y = (1 * glm::sin((S_WorldTime + ((float)i / 10000.0f)) / 100 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().y;
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().z = (1 * glm::cos((S_WorldTime + ((float)i / 10000.0f)) / 600 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().z;
	//		break;
	//	}
	//}
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
