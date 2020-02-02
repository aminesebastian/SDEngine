#pragma once

struct FInputKey {
	FInputKey() {
		bKeyDown = false;
		HeldTime = 0.0f;
	}
	bool bKeyDown;
	float HeldTime;
};
enum EMouseButton {
	LEFT,
	MIDDLE,
	RIGHT,
	BUTTON_4,
	BUTTON_5,
	NUM_MOUSE_BUTTONS
};
