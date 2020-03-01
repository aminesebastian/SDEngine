#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "SDL/SDL_scancode.h"
#include "Core/Input/InputUtilities.h"

class IUserInputReciever {
public:
	virtual void OnKeyDown(SDL_Scancode KeyCode) {}
	virtual void OnKeyUp(SDL_Scancode KeyCode) {}
	virtual void OnKeyHeld(SDL_Scancode KeyCode, float HeldTime) {}
	virtual void OnTextInput(const TString& Input) {}

	virtual void OnMouseButtonDown(vec2 ScreenPosition, EMouseButton Button) {}
	virtual void OnMouseButtonUp(vec2 ScreenPosition, EMouseButton Button) {}
	virtual void OnMouseAxis(vec2 ScreenPosition, vec2 Delta) {}
	virtual void OnMouseScrollAxis(float Delta) {}
};