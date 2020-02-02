#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "SDL/SDL_scancode.h"

class IUserInputReciever {
public:
	virtual void OnKeyDown(SDL_Scancode KeyCode) = 0;
	virtual void OnKeyUp(SDL_Scancode KeyCode) = 0;
	virtual void OnKeyHeld(SDL_Scancode KeyCode, float HeldTime) = 0;

	virtual void OnMouseButtonDown(vec2 ScreenPosition, EMouseButton Button) = 0;
	virtual void OnMouseButtonUp(vec2 ScreenPosition, EMouseButton Button) = 0;
	virtual void OnMouseAxis(vec2 ScreenPosition, vec2 Delta) = 0;
	virtual void OnMouseScrollAxis(float Delta) = 0;
};