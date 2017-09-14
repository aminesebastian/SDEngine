#include "Engine.h"
#include "Light.h"
#include "AssetManager.h"

Engine::Engine() {
	S_Display = new Display(WINDOW_WIDTH, WINDOW_HEIGHT, "SD_Engine", WINDOW_BIT_DEPTH);
	S_RenderingEngine = new URenderingEngine(S_Display);
	S_World = new UWorld();

	Transform cameraTransform(vec3(0, 15, 5));
	cameraTransform.SetRotation(50, -180, 0);
	S_Camera = new Camera(cameraTransform, 70, S_Display->GetAspectRatio(), 0.01f, 1000.0f);

	TwInit(TW_OPENGL, NULL);
	TwWindowSize(S_Display->GetDimensions().x, S_Display->GetDimensions().y);
	S_InfoBar = TwNewBar("StatUnit");
	TwDefine(" StatUnit refresh=0.1 ");
	TwDefine(" StatUnit alpha=30 ");
	TwAddVarRO(S_InfoBar, "Frame Time", TW_TYPE_FLOAT, &S_DeltaTime, "");
	TwAddVarRO(S_InfoBar, "Frame Rate", TW_TYPE_FLOAT, &S_FrameRate, "");

	S_DefaultMaterial = new Material("./Res/Shaders/DefaultGeometryPassShader");
	Texture2D* albedoTexture = new Texture2D("./Res/Textures/Checkboard1K.png");
	S_DefaultMaterial->SetTextureParameter("Albedo", albedoTexture);
	S_DefaultMaterial->SetVec3Parameter("Normal", vec3(0.5f, 0.5f, 1.0f));
	S_DefaultMaterial->SetScalarParameter("Roughness", 0.5f);
	S_DefaultMaterial->SetScalarParameter("Metalness", 0.5f);

	S_AssetManager = new AssetManager();
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

		GameLoop();
		RenderingLoop();
		UILoop();
		Movement();

		S_DeltaTime = now - S_LastFrameTime;
		S_FrameRate = 1.0f / (S_DeltaTime/1000);
		S_LastFrameTime = now;
		S_WorldTime += S_DeltaTime;
		S_Display->Update();
	}
}

/**********************************************************************************************//**
 * @fn	void Engine::GameLoop()
 *
 * @brief	Game loop.
 *
 * @author	Amine
 * @date	2/20/2017
 **************************************************************************************************/

void Engine::GameLoop() {
	S_World->TickWorld(S_DeltaTime);
}

/**********************************************************************************************//**
 * @fn	void Engine::RenderingLoop()
 *
 * @brief	Rendering loop.
 *
 * @author	Amine
 * @date	2/20/2017
 **************************************************************************************************/

void Engine::RenderingLoop() {
	//for (int i = 0; i < S_World->GetWorldLights().size(); i++) {
	//	switch (i % 5) {
	//	case 0:
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().x = (3 * glm::sin((S_WorldTime + ((float)i / 1000.0f)) / 500 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().x + (i % 10);
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().y = (3 * glm::sin((S_WorldTime + ((float)i / 1000.0f)) / 400 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().y;
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().z = (3 * glm::cos((S_WorldTime + ((float)i / 1000.0f)) / 600 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().z + (i % 10);
	//		break;
	//	case 1:
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().x = (-3 * glm::sin((S_WorldTime + ((float)i / 1000.0f)) / 400 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().x + (i % 10);
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().y = (-3 * glm::sin((S_WorldTime + ((float)i / 1000.0f)) / 500 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().y;
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().z = (-3 * glm::cos((S_WorldTime + ((float)i / 1000.0f)) / 600 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().z + (i % 10);
	//		break;
	//	case 2:
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().x = (3 * glm::sin((S_WorldTime + ((float)i / 1000.0f)) / 300 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().x + (i % 10);
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().y = (3 * glm::sin((S_WorldTime + ((float)i / 1000.0f)) / 400 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().y;
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().z = (3 * glm::cos((S_WorldTime + ((float)i / 1000.0f)) / 500 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().z + (i % 10);
	//		break;
	//	case 3:
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().x = (-3 * glm::sin((S_WorldTime + ((float)i / 1000.0f)) / 400 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().x + (i % 10);
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().y = (-3 * glm::sin((S_WorldTime + ((float)i / 1000.0f)) / 600 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().y;
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().z = (-3 * glm::cos((S_WorldTime + ((float)i / 1000.0f)) / 800 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().z + (i % 10);
	//		break;
	//	case 4:
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().x = (3 * glm::sin((S_WorldTime + ((float)i / 1000.0f)) / 500 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().x + (i % 10);
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().y = (3 * glm::sin((S_WorldTime + ((float)i / 1000.0f)) / 300 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().y;
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().z = (3 * glm::cos((S_WorldTime + ((float)i / 1000.0f)) / 600 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().z + (i % 10);
	//		break;
	//	default:
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().x = (3 * glm::sin((S_WorldTime + ((float)i / 1000.0f)) / 500 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().x + (i % 10);
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().y = (3 * glm::sin((S_WorldTime + ((float)i / 1000.0f)) / 300 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().y;
	//		S_World->GetWorldLights()[i]->GetTransform().GetPosition().z = (3 * glm::cos((S_WorldTime + ((float)i / 1000.0f)) / 600 * ((i + 1) / 5))) + S_World->GetWorldLights()[i]->GetInitialTransform().GetPosition().z + (i % 10);
	//		break;
	//	}
	//}
	S_RenderingEngine->RenderWorld(S_World, S_Camera);
}

/**********************************************************************************************//**
 * @fn	void Engine::UILoop()
 *
 * @brief	User interface loops this object.
 *
 * @author	Amine
 * @date	2/20/2017
 **************************************************************************************************/

void Engine::UILoop() {
	TwDraw();
}
void Engine::Movement() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			S_Display->CloseDisplay();
		}
		if (e.type == SDL_WINDOWEVENT_RESIZED) {
			S_Display->ResizeDisplay(e.window.data1, e.window.data2);
		}
		if (e.type == SDL_MOUSEMOTION) {
			if (e.motion.state & SDL_BUTTON_RMASK) {
				SDL_ShowCursor(0);
				S_Camera->AddOrbit((float)(e.motion.y - lastMouseY) / lookSpeed, -(float)(e.motion.x - lastMouseX) / lookSpeed);
				lastMouseX = e.motion.x;
				lastMouseY = e.motion.y;
			}
			else if (e.motion.state & SDL_BUTTON_MMASK) {
				SDL_ShowCursor(0);
				S_Camera->GetTransform().GetPosition().x -= (float)(e.motion.x - lastMouseX) / 250.0f;
				S_Camera->GetTransform().GetPosition().y += (float)(e.motion.y - lastMouseY) / 250.0f;
				lastMouseX = e.motion.x;
				lastMouseY = e.motion.y;
			}
			else {
				SDL_ShowCursor(1);
				lastMouseX = e.motion.x;
				lastMouseY = e.motion.y;
			}
		}
		if (e.type == SDL_MOUSEWHEEL) {
			movementSpeed = clamp(movementSpeed + ((float)e.wheel.y / 100.0f), 0.01f, 2.5f);
		}
		if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RETURN]) {
			S_RenderingEngine->RecompileShaders(S_World);
		}
		if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_F]) {
			S_Camera->SetTransform(S_Camera->GetInitialTransform());
		}
		keyInfo.bWDown = SDL_GetKeyboardState(NULL)[SDL_SCANCODE_W];
		keyInfo.bSDown = SDL_GetKeyboardState(NULL)[SDL_SCANCODE_S];
		keyInfo.bDDown = SDL_GetKeyboardState(NULL)[SDL_SCANCODE_A];
		keyInfo.bADown = SDL_GetKeyboardState(NULL)[SDL_SCANCODE_D];
		keyInfo.bQDown = SDL_GetKeyboardState(NULL)[SDL_SCANCODE_Q];
		keyInfo.bEDown = SDL_GetKeyboardState(NULL)[SDL_SCANCODE_E];
		if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_L]) {
			for (int i = 0; i < S_World->GetWorldLights().size(); i++) {
				S_World->GetWorldLights()[i]->ToggleDebug(!S_World->GetWorldLights()[i]->GetDebugMode());
			}
		}
	}
	if (keyInfo.bWDown) {
		S_Camera->GetTransform().GetPosition() = S_Camera->GetTransform().GetPosition() + S_Camera->GetTransform().GetForwardVector() * movementSpeed;
	}
	if (keyInfo.bSDown) {
		S_Camera->GetTransform().GetPosition() = S_Camera->GetTransform().GetPosition() - S_Camera->GetTransform().GetForwardVector() * movementSpeed;
	}
	if (keyInfo.bADown) {
		S_Camera->GetTransform().GetPosition() = S_Camera->GetTransform().GetPosition() - S_Camera->GetTransform().GetRightVector() * movementSpeed;
	}
	if (keyInfo.bDDown) {
		S_Camera->GetTransform().GetPosition() = S_Camera->GetTransform().GetPosition() + S_Camera->GetTransform().GetRightVector() * movementSpeed;
	}
	if (keyInfo.bQDown) {
		S_Camera->GetTransform().GetPosition() = S_Camera->GetTransform().GetPosition() - S_Camera->GetTransform().GetUpVector() * movementSpeed;
	}
	if (keyInfo.bEDown) {
		S_Camera->GetTransform().GetPosition() = S_Camera->GetTransform().GetPosition() + S_Camera->GetTransform().GetUpVector() * movementSpeed;
	}

}






