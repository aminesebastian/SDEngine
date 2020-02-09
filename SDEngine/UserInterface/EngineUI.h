#pragma once
#include "UserInterface/UserInterface.h"
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/DataStructures/DataStructures.h"
#include <SDL/SDL.h>

class PictorumWidget;

class EngineUI {
public:
	EngineUI();
	~EngineUI();

	bool InitalizeUI(SDL_Window& Window, const SDL_GLContext& Context);
	void UpdateUI(SDL_Window& Window);
	void RenderUI(const float& DeltaTime);
	bool DestroyUI();

	SArray<float> FrameTimeCache;
	int MaxFrameTimeCache;

private:
	float GetSmoothedFrameTime();

	PictorumWidget* SelectedWidget;
};

