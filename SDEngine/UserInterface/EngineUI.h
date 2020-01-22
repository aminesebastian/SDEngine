#pragma once
#include "UserInterface/UserInterface.h"
#include "Utilities/TypeDefenitions.h"

#include <SDL/SDL.h>

class EngineUI {
public:
	EngineUI();
	~EngineUI();

	bool InitalizeUI(SDL_Window* Window, SDL_GLContext Context);
	void UpdateUI(SDL_Window* Window);
	void RenderUI(float DeltaTime);
	bool DestroyUI();

	SArray<float> FrameTimeCache;
	int MaxFrameTimeCache;

private:
	float GetSmoothedFrameTime();
};

