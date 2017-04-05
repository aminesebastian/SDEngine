#include "Engine.h"

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
}
Engine::~Engine() {
	delete &S_Display;
	delete &S_RenderingEngine;
}
bool Engine::Init() {
	if (bIsInitialized) { return false; }

	S_RenderingEngine->ChangeShader("./Res/GeometryPassShader");
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	bIsInitialized = true;
	return true;
}

void Engine::MainLoop()  {
	while (!S_Display->IsClosed()) {
		long now = SDL_GetTicks();

		GameLoop();
		RenderingLoop();
		UILoop();
		Movement();

		S_DeltaTime = ((float)(now - S_LastFrameTime));
		S_FrameRate = 1.0f / S_DeltaTime;
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
			S_RenderingEngine->RecompileShaders();
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






