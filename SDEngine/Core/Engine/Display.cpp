#include "Core/Engine/Display.h"
#include <iostream>
#include "Core/Engine/Engine.h"
#include "Core/Utilities/Logger.h"


Display::Display(int Width, int Height, const std::string& Title, int BitDepth) {
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, BitDepth);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, BitDepth);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, BitDepth);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, BitDepth);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, BitDepth * 2);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, BitDepth * 4);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

#ifdef _GDEBUG
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif


	SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "1");

	Window = SDL_CreateWindow(Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	GLContext = SDL_GL_CreateContext(Window);

	GLenum status = glewInit();

	if (status != GLEW_OK) {
		std::cerr << "GL Failed To Initialize!" << std::endl;
	}

	bIsClosed = false;

#ifdef _GDEBUG
	// Enable the debug callback
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(openglCallbackFunction, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);
#endif

	// Capture the display state.
	vec2 resolution = glm::vec2(Width, Height);
	vec3 dpi;
	SDL_GetDisplayDPI(0, &dpi[2], &dpi[0], &dpi[1]);
	DisplayState = new FDisplayState(resolution, vec2(dpi.x, dpi.y), dpi.z);

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
	if (NewWidth != GetDimensions().x || NewHeight != GetDimensions().y) {
		SDL_SetWindowSize(Window, NewWidth, NewHeight);
		glViewport(0, 0, NewWidth, NewHeight);

		vec2 resolution = glm::vec2(NewWidth, NewHeight);
		vec3 dpi;
		SDL_GetDisplayDPI(0, &dpi[2], &dpi[0], &dpi[1]);
		DisplayState = new FDisplayState(resolution, vec2(dpi.x, dpi.y), dpi.z);

		Engine::GetInstance()->GetFocusedViewport()->ChangeRenderTargetDimensions(GetDimensions());
	}
}

void Display::openglCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	TString actualMessage(message, length);
	if (severity == GL_DEBUG_SEVERITY_LOW) {
		SD_ENGINE_WARN("OpenGL Warning: {0}", actualMessage);
	} else if (severity == GL_DEBUG_SEVERITY_MEDIUM) {
		SD_ENGINE_WARN("OpenGL Error: {0}", actualMessage);
	} else 	if (severity == GL_DEBUG_SEVERITY_HIGH) {
		SD_ENGINE_ERROR("OpenGL Fatal Error: {0}", actualMessage);
	}
}