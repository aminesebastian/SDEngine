#include "Display.h"
#include <GLEW\glew.h>
#include <iostream>
#include "Engine/Engine.h"


Display::Display(int Width, int Height, const std::string& Title, int BitDepth) {
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, BitDepth);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, BitDepth);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, BitDepth);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, BitDepth);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, BitDepth * 2);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, BitDepth * 4);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "1");

	Window = SDL_CreateWindow(Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	GLContext = SDL_GL_CreateContext(Window);

	GLenum status = glewInit();

	if (status != GLEW_OK) {
		std::cerr << "GL Failed To Initialize!" << std::endl;
	}

	bIsClosed = false;

	DisplayDimensions = glm::vec2(Width, Height);
	SDL_GL_SetSwapInterval(0);
}
Display::~Display() {
	SDL_GL_DeleteContext(GLContext);
	SDL_DestroyWindow(Window);

	SDL_Quit();
}
void Display::CloseDisplay() {
	bIsClosed = true;
}
SDL_Window* Display::GetWindow() {
	return Window;
}
SDL_GLContext Display::GetContext() {
	return GLContext;
}
bool Display::IsClosed() {
	return bIsClosed;
}
void Display::Clear(float R, float G, float B, float A) {
	glClearColor(R, G, B, A);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Display::Update() {
	SDL_GL_SwapWindow(Window);

	//SDL_Event e;
}
void Display::WindowResized(int NewWidth, int NewHeight) {
	if (NewWidth != DisplayDimensions.x || NewHeight != DisplayDimensions.y) {
		DisplayDimensions = glm::vec2(NewWidth, NewHeight);
		SDL_SetWindowSize(Window, NewWidth, NewHeight);
		glViewport(0, 0, NewWidth, NewHeight);
		Engine::GetInstance()->GetFocusedViewport()->ChangeRenderTargetDimensions(GetDimensions());
	}
}
