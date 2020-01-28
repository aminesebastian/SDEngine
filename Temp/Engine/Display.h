#pragma once
#include <string>
#include <SDL/SDL.h>
#include <GLM/glm.hpp>

class Display {

public:
	Display(int Width, int Height, const std::string& Title, int BitDepth);
	Display() : Display(1280, 720, "SD_ENGINE", 32) {}
	virtual ~Display();

	void Update();
	void Clear(float R, float G, float B, float A);
	bool IsClosed();
	void WindowResized(int NewWidth, int NewHeight);
	void CloseDisplay();
	SDL_Window* GetWindow();
	SDL_GLContext GetContext();

	inline glm::vec2 GetDimensions() const {
		return DisplayDimensions;
	}
	inline float GetAspectRatio() const {
		return ((float)DisplayDimensions.x) / ((float)DisplayDimensions.y);
	}
private:
	Display(const Display& other) {}
	void operator=(const Display& other) {}

	SDL_Window* Window;
	SDL_GLContext GLContext;
	bool bIsClosed;

	glm::vec2 DisplayDimensions;
};

