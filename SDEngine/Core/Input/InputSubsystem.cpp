#include "InputSubsystem.h"
#include "Core/Input/IUserInputReciever.h"
#include "Core/Engine/Engine.h"

#include "SDL/SDL_scancode.h"
#include "Lib/Imgui/imgui.h"
#include "Lib/Imgui/imgui_impl_sdl.h"
#include "Lib/Imgui/imgui_impl_opengl3.h"

InputSubsystem::InputSubsystem() {
	for (int i = 0; i < SDL_NUM_SCANCODES; i++) {
		InputKeyStates.Add(new FInputKey());
	}
	for (int i = 0; i < NUM_MOUSE_BUTTONS; i++) {
		MouseButtonStates.Add(new FInputKey());
	}
}
InputSubsystem::~InputSubsystem() {
	for (int i = 0; i < SDL_NUM_SCANCODES; i++) {
		delete InputKeyStates[i];
	}
}

void InputSubsystem::Initialize() {

}
void InputSubsystem::Tick(float DeltaTime) {
	ProcessInputHeldEvents(DeltaTime);
}

bool InputSubsystem::IsKeyDown(SDL_Scancode Key) {
	return InputKeyStates[Key]->bKeyDown;
}
float InputSubsystem::GetKeyHeldTime(SDL_Scancode Key) {
	return InputKeyStates[Key]->HeldTime;
}
bool InputSubsystem::IsMouseButtonDown(EMouseButton Button) {
	return MouseButtonStates[Button]->bKeyDown;
}
float InputSubsystem::GetMouseButtonHeldTime(EMouseButton Button) {
	return MouseButtonStates[Button]->HeldTime;
}
vec2 InputSubsystem::GetMousePosition() {
	return MousePosition;
}
vec2 InputSubsystem::GetMouseDelta() {
	return MouseDelta;
}

bool InputSubsystem::RegisterInputReciever(IUserInputReciever* Reciever) {
	return InputRecievers.AddUnique(Reciever);
}
bool InputSubsystem::DeregisterInputReciever(IUserInputReciever* Reciever) {
	return InputRecievers.Remove(Reciever);
}

void InputSubsystem::ProcessInputEvent(SDL_Event Event) {
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse || io.WantCaptureKeyboard) {
		ImGui_ImplSDL2_ProcessEvent(&Event);
		return;
	}

	switch (Event.type) {
		case SDL_KEYDOWN:
			ProcessKeyDown(Event.key.keysym.scancode);
			break;
		case SDL_KEYUP:
			ProcessKeyUp(Event.key.keysym.scancode);
			break;
		case SDL_MOUSEBUTTONDOWN:
			ProcessMouseDownInput((EMouseButton)(Event.button.button-1)); //Subtract 1 because SDL is 1 indexed
			break;
		case SDL_MOUSEBUTTONUP:
			ProcessMouseUpInput((EMouseButton)(Event.button.button-1)); //Subtract 1 because SDL is 1 indexed
			break;
		case SDL_MOUSEMOTION:
			MousePosition = vec2(Event.motion.x, Engine::GetInstance()->GetFocusedViewport()->GetRenderTargetDimensions().y - Event.motion.y);
			ProcessMouseMovement(MousePosition);
			MouseDelta = MousePosition - LastMousePosition;
			LastMousePosition = MousePosition;
			break;
		case SDL_MOUSEWHEEL:
			ProcessMouseScroll((float)Event.wheel.y);
			break;
		default:
			break;
	}
}
void InputSubsystem::ProcessMouseDownInput(EMouseButton Button) {
	MouseButtonStates[Button]->bKeyDown = true;
	for (IUserInputReciever* reciever : InputRecievers) {
		reciever->OnMouseButtonDown(MousePosition, Button);
	}
}
void InputSubsystem::ProcessMouseUpInput(EMouseButton Button) {
	MouseButtonStates[Button]->bKeyDown = false;
	for (IUserInputReciever* reciever : InputRecievers) {
		reciever->OnMouseButtonUp(MousePosition, Button);
	}
	MouseButtonStates[Button]->HeldTime = 0.0f;
}
void InputSubsystem::ProcessMouseMovement(vec2 ScreenPosition) {
	for (IUserInputReciever* reciever : InputRecievers) {
		reciever->OnMouseAxis(MousePosition, MouseDelta);
	}
}
void InputSubsystem::ProcessMouseScroll(float Delta) {
	for (IUserInputReciever* reciever : InputRecievers) {
		reciever->OnMouseScrollAxis(Delta);
	}
}
void InputSubsystem::ProcessKeyDown(SDL_Scancode KeyCode) {
	InputKeyStates[KeyCode]->bKeyDown = true;
	for (IUserInputReciever* reciever : InputRecievers) {
		reciever->OnKeyDown(KeyCode);
	}
}
void InputSubsystem::ProcessKeyUp(SDL_Scancode KeyCode) {
	InputKeyStates[KeyCode]->bKeyDown = false;
	for (IUserInputReciever* reciever : InputRecievers) {
		reciever->OnKeyUp(KeyCode);
	}
	InputKeyStates[KeyCode]->HeldTime = 0.0f;
}
void InputSubsystem::ProcessInputHeldEvents(float DeltaTime) {
	for (int i = 0; i < InputKeyStates.Count(); i++) {
		FInputKey* key = InputKeyStates[i];
		if (key->bKeyDown) {
			for (IUserInputReciever* reciever : InputRecievers) {
				reciever->OnKeyHeld((SDL_Scancode)i, 0.0f);
			}
		}
		key->HeldTime += DeltaTime;
	}
	for (FInputKey* mouseKey : MouseButtonStates) {
		mouseKey->HeldTime += DeltaTime;
	}
}