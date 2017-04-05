#pragma once
#include <string>
#include <SDL/SDL.h>
#include <GLM/glm.hpp>
#include <AntTweakBar.h>

class Display {

public:
	Display(int Width, int Height, const std::string& Title, int BitDepth);
	Display() : Display(1280, 720, "SD_ENGINE", 32) {}
	virtual ~Display();

	void Update();
	void Clear(float R, float G, float B, float A);
	bool IsClosed();

	inline glm::vec2 GetDimensions() const{
		return glm::vec2(S_Width, S_Height);
	}
	inline float GetAspectRatio() const {
		return ((float)S_Width) / ((float)S_Height);
	}
	void CloseDisplay() { bIsClosed = true; }
private:
	Display(const Display& other) {}
	void operator=(const Display& other) {}

	SDL_Window* S_Window;
	SDL_GLContext S_GLContext;
	bool bIsClosed;

	int S_Width;
	int S_Height;
};

