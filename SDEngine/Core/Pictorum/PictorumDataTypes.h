#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Utilities/Logger.h"

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
struct FRenderGeometry {
	FRenderGeometry() {
		AllotedSpace = vec2(0.0f, 0.0f);
		Location = vec2(0.0f, 0.0f);
	}
	explicit FRenderGeometry(const vec2& RenderResolution, const vec2& RenderTargetDPI) : RenderResolution(RenderResolution), RenderTargetDPI(RenderTargetDPI) {}

	vec2 GetLocation(EPictorumLocationBasis Basis = EPictorumLocationBasis::ABSOLUTE) const {
		if (Basis == EPictorumLocationBasis::ABSOLUTE) {
			return Location;
		} else if (Basis == EPictorumLocationBasis::RELATIVE) {
			return Location / RenderResolution;
		} else if (Basis == EPictorumLocationBasis::NDC) {
			return ((Location / RenderResolution) - 0.5f) * 2.0f;
		}
		return Location;
	}
	/**
	* Sets a location in absolute coordinates.
	*
	* @param 	{vec2}	LocationIn	The location in absolute coordinates.
	*/
	void SetLocation(vec2 LocationIn) {
		Location = LocationIn;
	}
	/**
	* Adds an offset to the location.
	*
	* @param 	{float}	X						 	The X offset.
	* @param 	{float}	Y						 	The Y offset.
	* @param 	{EPictorumLocationBasis}	Basis	(Optional) The basis the offsets are supplied in.
	*/
	void AddLocation(float X, float Y, EPictorumLocationBasis Basis = EPictorumLocationBasis::ABSOLUTE) {
		Location.x += X;
		Location.y += Y;
	}
	/**
	* Gets allocated space in the provided basis.
	*
	* @param 	{EPictorumScaleBasis}	Basis	The basis to return the space in.
	*
	* @returns	{vec2}	The allocated space.
	*/
	vec2 GetAllotedSpace(EPictorumScaleBasis Basis = EPictorumScaleBasis::ABSOLUTE) const {
		if (Basis == EPictorumScaleBasis::ABSOLUTE) {
			return AllotedSpace;
		} else if (Basis == EPictorumScaleBasis::RELATIVE) {
			return AllotedSpace / RenderResolution;
		}
		return AllotedSpace;
	}
	/**
	* Sets allocated space in absolute coordinates.
	*
	* @param 	{vec2}	AllotedSpace	The space in absolute coordinates.
	*/
	void SetAllotedSpace(vec2 Space) {
		AllotedSpace = Space;
	}
	/**
	* Adds an offset to the alloted space.
	*
	* @param 	{float}	X						 	The X offset.
	* @param 	{float}	Y						 	The Y offset.
	* @param 	{EPictorumScaleBasis}	Basis	(Optional) The basis the offsets are supplied in.
	*/
	void AddAllotedSpace(float X, float Y, EPictorumScaleBasis Basis = EPictorumScaleBasis::ABSOLUTE) {
		AllotedSpace.x += X;
		AllotedSpace.y += Y;
	}
	/**
	* Gets render resolution in absolute coordinates.
	*
	* @returns	{const vec2&}	The render resolution in absolute coordinates.
	*/
	const vec2& GetRenderResolution() const {
		return RenderResolution;
	}
	/**
	* Sets render resolution in absolute coordinates.
	*
	* @param 	{const vec2&}	Resolution	The resolution in absolute coordinates.
	*/
	void SetRenderResolution(const vec2& Resolution) {
		RenderResolution = Resolution;
	}


	const float GetAllotedSpaceAspectRatio() const {
		return AllotedSpace.x / AllotedSpace.y;
	}
	const float GetRenderResolutionAspectRatio() const {
		return RenderResolution.x / RenderResolution.y;
	}
	const vec2& GetDPI() const {
		return RenderTargetDPI;
	}
	void SetDPI(const vec2& DPI) {
		RenderTargetDPI = DPI;
	}
private:
	/** The render resolution of the total render target. */
	vec2 RenderResolution;
	/** The render target DPI. */
	vec2 RenderTargetDPI;
	/** The alloted space for this widget to be rendered in (in absolute pixel space). */
	vec2 AllotedSpace;
	/** The location this space exists at (int absolute pixel space). */
	vec2 Location;
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
	void ApplyPaddingToGeometry(const FRenderGeometry& OriginalRenderGeometry, FRenderGeometry& OutRenderGeometry) const {
		OutRenderGeometry.AddLocation(Left, Bottom);
		OutRenderGeometry.AddAllotedSpace(-Right * 2, -Top * 2);
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
		Top = 1.0f;
		Right = 1.0f;
		Bottom = 0.0f;
		Left = 0.0f;
		bIsRelative = true;
	}
	void SetRelative() {
		bIsRelative = true;
	}
	void SetAbsolute() {
		bIsRelative = false;
	}

	void ApplyAnchorsToGeometry(const FRenderGeometry& OriginalRenderGeometry, FRenderGeometry& OutRenderGeometry) const {
		if (Left >= Right) {
			SD_ENGINE_WARN("An anchor's Left value is greater than or equal to its Right value. This will result in negative allocated X space.");
			return;
		}
		if (Bottom >= Top) {
			SD_ENGINE_WARN("An anchor's Bottom value is greater than or equal to its Top value. This will result in negative allocated Y space.");
			return;
		}

		vec2 newLocation;
		vec2 newSpace;
		if (bIsRelative) {
			newLocation.x = OriginalRenderGeometry.GetLocation().x + (OriginalRenderGeometry.GetAllotedSpace().x * Left);
			newLocation.y = OriginalRenderGeometry.GetLocation().y + (OriginalRenderGeometry.GetAllotedSpace().y * Bottom);

			newSpace.x = (Right - Left) * OriginalRenderGeometry.GetAllotedSpace().x;
			newSpace.y = (Top - Bottom) * OriginalRenderGeometry.GetAllotedSpace().y;
		} else {
			newLocation.x = OriginalRenderGeometry.GetLocation().x + Left;
			newLocation.y = OriginalRenderGeometry.GetLocation().x + Bottom;

			newSpace.x = ((Right - Left) / OriginalRenderGeometry.GetRenderResolution().x) * OriginalRenderGeometry.GetAllotedSpace().x;
			newSpace.y = ((Top - Bottom) / OriginalRenderGeometry.GetRenderResolution().y) * OriginalRenderGeometry.GetAllotedSpace().y;
		}
		OutRenderGeometry.SetLocation(newLocation);
		OutRenderGeometry.SetAllotedSpace(newSpace);
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
	void ApplyMarginsToGeometry(const FRenderGeometry& OriginalRenderGeometry, FRenderGeometry& OutRenderGeometry) const {
		OutRenderGeometry.AddLocation(Left, Bottom);
		OutRenderGeometry.AddAllotedSpace(-Right * 2, -Top * 2);
	}
	float& operator[](const int32 Index) {
		return (*this)[Index];
	}
};