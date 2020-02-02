#pragma once
#include "Core/DataTypes/TypeDefenitions.h"

enum class EVerticalAlignment : uint8 {
	LEFT,
	CENTER,
	RIGHT
};
enum class EHorizontalAlignment : uint8 {
	LEFT,
	CENTER,
	RIGHT
};
enum class EFillState : uint8 {
	AUTOMATIC,
	FILL_ALL_SPACE
};
enum class EPictorumVisibilityState : uint8 {
	COLLAPSED,
	HIDDEN,
	VISIBLE,
	HIT_TEST_INVISIBLE,
	SELF_HIT_TEST_INVISIBLE
};
struct FUserInterfaceEvent {
	FUserInterfaceEvent() {
		bHandled = false;
		bCaptureMouse = false;
	}

	bool GetHandled() { return bHandled; }
	void SetHandled() { bHandled = true; }
	void SetUnhandled() { bHandled = false; }

	bool ShouldCaptureMouse() { return bCaptureMouse; }
	void CaptureMouse() { bCaptureMouse = true; }
private:
	bool bHandled;
	bool bCaptureMouse;
};
struct FPadding {
	float Top;
	float Right;
	float Bottom;
	float Left;

	FPadding() {
		Top = 0.0f;
		Right = 0.0f;
		Bottom = 0.0f;
		Left = 0.0f;
	}
	float& operator[](const int32 Index) {
		return (*this)[Index];
	}
};
struct FAnchors {
	float Top;
	float Right;
	float Bottom;
	float Left;
	bool bIsRelative;

	FAnchors() {
		Top = 0.0f;
		Right = 0.0f;
		Bottom = 0.0f;
		Left = 0.0f;
		bIsRelative = false;
	}
	void SetRelative() {
		bIsRelative = true;
	}
	void SetAbsolute() {
		bIsRelative = false;
	}
	bool IsRelative() {
		return bIsRelative;
	}
	float& operator[](const int32 Index) {
		return (*this)[Index];
	}
};
struct FMargins {
	float Top;
	float Right;
	float Bottom;
	float Left;

	FMargins() {
		Top = 0.0f;
		Right = 0.0f;
		Bottom = 0.0f;
		Left = 0.0f;
	}
	float& operator[](const int32 Index) {
		return (*this)[Index];
	}
};
struct FRenderGeometry {
	vec2 RenderResolution;

	FRenderGeometry() {
		RenderResolution = vec2(0.0f, 0.0f);
	}
};