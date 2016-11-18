#pragma once
#include <string>
#include <SDL/SDL.h>

class Display {

public:
	Display(int Width, int Height, const std::string& Title, int BitDepth);
	virtual ~Display();

	void Update();
	void Clear(float R, float G, float B, float A);
	bool IsClosed();
private:
	Display(const Display& other) {}
	void operator=(const Display& other) {}

	SDL_Window* S_Window;
	SDL_GLContext S_GLContext;
	bool bIsClosed;
};

