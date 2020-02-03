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
enum class EPictorumLocationBasis : uint8 {
	ABSOLUTE,
	RELATIVE,
	NDC
};
enum class EPictorumScaleBasis : uint8 {
	ABSOLUTE,
	RELATIVE
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
	FRenderGeometry() {
		RenderResolution = vec2(0.0f, 0.0f);
		AllotedSpace = vec2(0.0f, 0.0f);
		Location = vec2(0.0f, 0.0f);
	}
	explicit FRenderGeometry(vec2 RenderTargetResolution) {
		RenderResolution = RenderTargetResolution;
	}
	vec2 GetLocation(EPictorumLocationBasis Basis) const {
		if (Basis == EPictorumLocationBasis::ABSOLUTE) {
			return Location;
		} else if (Basis == EPictorumLocationBasis::RELATIVE) {
			return Location / RenderResolution;
		} else if (Basis == EPictorumLocationBasis::NDC) {
			return ((Location / RenderResolution) - 0.5f) * 2.0f;
		}
		return Location;
	}
	void SetLocation(vec2 LocationIn) {
		Location = LocationIn;
	}
	vec2 GetAllocatedSpace(EPictorumScaleBasis Basis) const {
		if (Basis == EPictorumScaleBasis::ABSOLUTE) {
			return AllotedSpace;
		} else if (Basis == EPictorumScaleBasis::RELATIVE) {
			return AllotedSpace / RenderResolution;
		}
		return AllotedSpace;
	}
	void SetAllocatedSpace(vec2 Space) {
		AllotedSpace = Space;
	}
	vec2 GetRenderResolution() const {
		return RenderResolution;
	}
	void SetRenderResolution(vec2 Resolution) {
		RenderResolution = Resolution;
	}
	float GetSpaceAspectRatio() const {
		return AllotedSpace.x / AllotedSpace.y;
	}
	float GetRenderResolutionAspectRatio() const {
		return RenderResolution.x / RenderResolution.y;
	}
private:
	/** The render resolution of the total render target. */
	vec2 RenderResolution;
	/** The alloted space for this widget to be rendered in (in absolute pixel space). */
	vec2 AllotedSpace;
	/** The location this space exists at (int absolute pixel space). */
	vec2 Location;
};