#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Engine/Delegates/Event.h"
#include "Core/Engine/WindowDataTypes.h"
#include "Core/Input/IUserInputReciever.h"
#include "Core/Utilities/Logger.h"
#include <GLEW\glew.h>
#include <GLM/glm.hpp>
#include <SDL/SDL.h>

using namespace glm;

class Window: public IUserInputReciever {

public:
	Window(const TString& Title, const vec2& Dimensions, const int& BitDepth = 8);
	virtual ~Window();

	virtual void Initialize();
	void Update();
	void Close();

	SDL_Window* GetWindow() const;
	SDL_GLContext& GetGraphicsContext();

	const vec2& GetDimensions() const;
	const vec2& GetDisplayDPI() const;
	const TString& GetTitle() const;
	const float& GetFrameTime() const;
	const float& GetFrameRate() const;
	const int32& GetWindowId() const;
	const FDisplayState& GetDisplayState() const;
	const bool& IsClosed();

	Event<void(const int32&, const FDisplayState&)> OnWindowResized;
	Event<void(const int32&, const FDisplayState&)> OnWindowMaximized;
	Event<void(const int32&, const FDisplayState&)> OnWindowRestored;
	Event<void(const int32&)> OnWindowMinimized;
	Event<void(const int32&)> OnWindowClosed;

protected:
	virtual void UpdateInternal(const float& DeltaTime);
	virtual void WindowResized(const int32& NewWidth, const int32& NewHeight);
	virtual void WindowMaximized(const int32& NewWidth, const int32& NewHeight);
	virtual void WindowRestored(const int32& NewWidth, const int32& NewHeight);
	virtual void WindowMinimized();

	void UpdateDisplayState(const int32& Width, const int32& Height);

	static void OpenglCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

	Window(const Window& other) = delete;
	void operator=(const Window& other) = delete;

private:
	friend class Engine;

	SDL_Window* _Window;
	SDL_GLContext _Context;

	bool Closed;
	FDisplayState* DisplayState;
	int32 BitDepth;
	TString Title;
	int32 WindowId;
	float DeltaTime;
	float FrameRate;
	Uint64 LastFrameTime;
};

