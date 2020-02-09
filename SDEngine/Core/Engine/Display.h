#pragma once
#include <string>
#include <GLEW\glew.h>
#include <SDL/SDL.h>
#include <GLM/glm.hpp>

using namespace glm;

struct FDisplayState {
	FDisplayState(const vec2& Resolution, const vec2& DotsPerInch, const float& DiagonalDotsPerInch) : Resolution(Resolution), DotsPerInch(DotsPerInch), DiagonalDotsPerInch(DiagonalDotsPerInch) {}

	const vec2& GetResolution() const {
		return Resolution;
	}
	const vec2& GetDotsPerInch() const {
		return DotsPerInch;
	}
	const float& GetDiagonalDotsPerInch() const {
		return DiagonalDotsPerInch;
	}
private:
	const vec2 Resolution;
	const vec2 DotsPerInch;
	const float DiagonalDotsPerInch;
};

class Display {

public:
	Display(int Width, int Height, const std::string& Title, int BitDepth);
	virtual ~Display();

	void Update();
	bool IsClosed();
	void WindowResized(int NewWidth, int NewHeight);
	void CloseDisplay();
	SDL_Window* GetWindow();
	SDL_GLContext GetContext();

	/**
	 * Gets the resolution of the display.
	 *
	 * @returns	{const glm::vec2&}	The display resolution.
	 */
	inline const vec2& GetDimensions() const {
		return DisplayState->GetResolution();
	}

	/**
	 * Gets display DPI.
	 * x: The Horizontal DPI
	 * y: The Vertical DPI  
	 *
	 * @returns	{const glm::vec2&}	The packed display DPI.
	 */
	inline const vec2& GetDisplayDPI() const {
		return DisplayState->GetDotsPerInch();
	}
	inline const FDisplayState& GetDisplayState() const {
		return *DisplayState;
	}
	inline const float GetAspectRatio() const {
		return ((float)DisplayState->GetResolution().x) / ((float)DisplayState->GetResolution().y);
	}

private:
	Display(const Display& other) = delete;
	void operator=(const Display& other) = delete;

	SDL_Window* Window;
	SDL_GLContext GLContext;
	bool bIsClosed;
	FDisplayState* DisplayState;

	static void OpenglCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
};

