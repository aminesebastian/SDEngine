#include "Core/Engine/Window.h"
#include "Core/Engine/Engine.h"
#include "Core/Pictorum/PictorumRenderer.h"
#include "Core/Utilities/Logger.h"


Window::Window(const TString& Title, const vec2& Dimensions, const int& BitDepth) : BitDepth(BitDepth), Title(Title) {
	// Initialize SDL.
	SDL_Init(SDL_INIT_EVERYTHING);

	// Set the SDL attributes for bitdepth, etc.
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

	// Create the window and OpenGL context. Cache the window Id.
	_Window = SDL_CreateWindow(Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int32)Dimensions.x, (int32)Dimensions.y, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_BORDERLESS | SDL_WINDOW_HIDDEN);
	_Context = SDL_GL_CreateContext(_Window);
	WindowId = SDL_GetWindowID(_Window);

	// Initialize GLEW.
	GLenum status = glewInit();
	if (status != GLEW_OK) {
		SD_ENGINE_ERROR("Failed to initialize GL context.");
	}

#ifdef _GDEBUG
	// Enable the debug callback
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(OpenglCallbackFunction, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);
#endif

	// Capture the display state.
	UpdateDisplayState((int32)Dimensions.x, (int32)Dimensions.y);
	SDL_GL_SetSwapInterval(0);

	DeltaTime     = 0.0f;
	LastFrameTime = SDL_GetPerformanceCounter();

	SDL_HideWindow(_Window);

	// Mark this display as open.
	Closed = false;
}
Window::~Window() {
	// Delete the OpenGL context and then destroy the window. 
	SDL_GL_DeleteContext(_Context);
	SDL_DestroyWindow(_Window);

	// THIS SHOULD BE REMOVED.
	SDL_Quit();
}
void Window::Initialize() {
	SDL_ShowWindow(_Window);
}
void Window::Update() {
	Uint64 frameStartTime = SDL_GetPerformanceCounter();

	UpdateInternal(DeltaTime);
	SDL_GL_SwapWindow(_Window);

	DeltaTime = (float)(frameStartTime - LastFrameTime) / SDL_GetPerformanceFrequency();
	FrameRate = 1.0f / DeltaTime;
	LastFrameTime = frameStartTime;
}
void Window::UpdateInternal(const float& DeltaTime) {

}
void Window::Close() {
	OnWindowClosed.Broadcast(WindowId);
	Closed = true;
}

void Window::WindowResized(const int32& NewWidth, const int32& NewHeight) {
	SDL_SetWindowSize(_Window, NewWidth, NewHeight);
	glViewport(0, 0, NewWidth, NewHeight);
	UpdateDisplayState(NewWidth, NewHeight);
	OnWindowResized.Broadcast(WindowId, *DisplayState);
}
void Window::WindowMaximized(const int32& NewWidth, const int32& NewHeight) {
	OnWindowMaximized.Broadcast(WindowId, *DisplayState);
}
void Window::WindowRestored(const int32& NewWidth, const int32& NewHeight) {
	OnWindowRestored.Broadcast(WindowId, *DisplayState);
}
void Window::WindowMinimized() {
	OnWindowMinimized.Broadcast(WindowId);
}

void Window::UpdateDisplayState(const int32& Width, const int32& Height) {
	vec3 dpi;
	SDL_GetDisplayDPI(0, &dpi[2], &dpi[0], &dpi[1]);
	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);
	DisplayState = new FDisplayState(vec2(Width, Height), vec2(dpi.x, dpi.y), dpi.z, displayMode.refresh_rate);
}

const vec2& Window::GetDimensions() const {
	return DisplayState->GetResolution();
}
const vec2& Window::GetDisplayDPI() const {
	return DisplayState->GetDotsPerInch();
}
const TString& Window::GetTitle() const {
	return Title;
}
const float& Window::GetFrameTime() const {
	return DeltaTime;
}
const float& Window::GetFrameRate() const {
	return FrameRate;
}
const int32& Window::GetWindowId() const {
	return WindowId;
}
const FDisplayState& Window::GetDisplayState() const {
	return *DisplayState;
}
const bool& Window::IsClosed() {
	return Closed;
}
SDL_Window* Window::GetWindow() const {
	return _Window;
}
SDL_GLContext& Window::GetGraphicsContext() {
	return _Context;
}
void Window::OpenglCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	TString actualMessage(message, length);
	if (severity == GL_DEBUG_SEVERITY_LOW) {
		SD_ENGINE_WARN("OpenGL Warning: {0}", actualMessage);
	} else if (severity == GL_DEBUG_SEVERITY_MEDIUM) {
		SD_ENGINE_WARN("OpenGL Error: {0}", actualMessage);
	} else 	if (severity == GL_DEBUG_SEVERITY_HIGH) {
		SD_ENGINE_ERROR("OpenGL Fatal Error: {0}", actualMessage);
	}
}