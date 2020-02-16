#pragma once
#include "SDL/SDL_events.h"
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/DataStructures/Array.h"
#include "Core/Input/InputUtilities.h"

class IUserInputReciever;

enum class EMouseCursorStyle : uint8 {
	Arrow,
	IBeam,
	Wait,
	Crosshair,
	WaitArrow,
	SizeNWSW,
	SizeNESW,
	SizeHorizontal,
	SizeVertical,
	SizeALL,
	Disabled,
	Hand
};

class InputSubsystem {
public:
	InputSubsystem();
	~InputSubsystem();

	void Initialize();
	void Tick(float DeltaTime);
	void ProcessInputEvent(SDL_Event Event);

	bool RegisterInputReciever(IUserInputReciever* Reciever);
	bool DeregisterInputReciever(IUserInputReciever* Reciever);

	bool IsKeyDown(SDL_Scancode Key);
	float GetKeyHeldTime(SDL_Scancode Key);
	bool IsMouseButtonDown(EMouseButton Button);
	float GetMouseButtonHeldTime(EMouseButton Button);

	vec2 GetMousePosition();
	vec2 GetMouseDelta();

	void SetMouseCursorStyle(const EMouseCursorStyle& Cursor);
	const EMouseCursorStyle& GetMouseCursorStyle() const;
	void CaptureMouseCursor();
	void ReleaseMouseCursor();
	const bool IsMouseCursorCaptured() const;
	void HideMouseCursor();
	void ShowMouseCursor();
	const bool IsMouseCursorHidden() const;
private:
	SArray<IUserInputReciever*> InputRecievers;

	SArray<FInputKey*> InputKeyStates;
	SArray<FInputKey*> MouseButtonStates;

	vec2 MousePosition;
	vec2 LastMousePosition;
	vec2 MouseDelta;
	vec2 MouseDownPosition;

	bool bCursorCaptured;
	bool bCursorHidden;

	EMouseCursorStyle CurrentCursorStyle;
	SDL_Cursor* MouseCursor;

	void ProcessMouseDownInput(EMouseButton Button);
	void ProcessMouseUpInput(EMouseButton Button);
	void ProcessMouseMovement(vec2 ScreenPosition);
	void ProcessMouseScroll(float Delta);

	void ProcessKeyDown(SDL_Scancode KeyCode);
	void ProcessKeyUp(SDL_Scancode KeyCode);

	void ProcessInputHeldEvents(float DeltaTime);
};

