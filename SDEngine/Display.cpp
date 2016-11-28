#include "Display.h"
#include <GLEW\glew.h>
#include <iostream>


Display::Display(int Width, int Height, const std::string& Title, int BitDepth) {
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, BitDepth);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, BitDepth);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, BitDepth);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, BitDepth);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, BitDepth*2);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, BitDepth*4);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	S_Window = SDL_CreateWindow(Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height, SDL_WINDOW_OPENGL);
	S_GLContext = SDL_GL_CreateContext(S_Window);

	GLenum status = glewInit();

	if (status != GLEW_OK) {
		std::cerr << "GL Failed To Initialize!" << std::endl;
	}

	bIsClosed = false;


	S_Width = Width;
	S_Height = Height;
	SDL_GL_SetSwapInterval(0);
}
Display::~Display() {
	SDL_GL_DeleteContext(S_GLContext);
	SDL_DestroyWindow(S_Window);

	SDL_Quit();
}
bool Display::IsClosed() {
	return bIsClosed;
}
void Display::Clear(float R, float G, float B, float A) {
	glClearColor(R, G, B, A);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Display::Update() {
	SDL_GL_SwapWindow(S_Window);

	//SDL_Event e;
}
