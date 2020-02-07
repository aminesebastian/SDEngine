#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Utilities/Logger.h"

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
enum class EPictorumSide : uint8 {
	TOP,
	RIGHT,
	BOTTOM,
	LEFT
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
struct FAnchors {
	FAnchors() {
		Sides[0] = 1.0f;
		Sides[1] = 1.0f;
		Sides[2] = 0.0f;
		Sides[3] = 0.0f;
		memset(&Relative[0], 1, 4);
	}
	void SetRelative(const EPictorumSide& Side) {
		Relative[(uint8)Side] = true;
	}
	void SetAbsolute(const EPictorumSide& Side) {
		Relative[(uint8)Side] = false;
	}
	const bool& IsSideRelative(const EPictorumSide& Side) const {
		return Relative[(uint8)Side];
	}
	const bool& IsSideAbsolute(const EPictorumSide& Side) const {
		return !Relative[(uint8)Side];
	}
	const float& GetTop() const {
		return Sides[0];
	}
	FAnchors& SetTop(const float& Value) {
		Sides[0] = Value;
		return *this;
	}
	const float& GetRight() const {
		return Sides[2];
	}
	FAnchors& SetRight(const float& Value) {
		Sides[1] = Value;
		return *this;
	}
	const float& GetBottom() const {
		return Sides[3];
	}
	FAnchors& SetBottom(const float& Value) {
		Sides[2] = Value;
		return *this;
	}
	const float& GetLeft() const {
		return Sides[3];
	}
	FAnchors& SetLeft(const float& Value) {
		Sides[3] = Value;
		return *this;
	}
	virtual void ApplyToGeometry(const FRenderGeometry& OriginalRenderGeometry, FRenderGeometry& OutRenderGeometry) const {
		float relativeTop    = GetSideRelativeValue(EPictorumSide::TOP, OriginalRenderGeometry.GetRenderResolution());
		float relativeRight  = GetSideRelativeValue(EPictorumSide::RIGHT, OriginalRenderGeometry.GetRenderResolution());
		float relativeBottom = GetSideRelativeValue(EPictorumSide::BOTTOM, OriginalRenderGeometry.GetRenderResolution());
		float relativeLeft   = GetSideRelativeValue(EPictorumSide::LEFT, OriginalRenderGeometry.GetRenderResolution());

		if (relativeLeft >= relativeRight) {
			SD_ENGINE_WARN("An anchor's Left value is greater than or equal to its Right value. This will result in negative allocated X space.");
			return;
		}
		if (relativeBottom >= relativeTop) {
			SD_ENGINE_WARN("An anchor's Bottom value is greater than or equal to its Top value. This will result in negative allocated Y space.");
			return;
		}

		vec2 newLocation;
		vec2 newSpace;

		newLocation.x = OriginalRenderGeometry.GetLocation().x + (OriginalRenderGeometry.GetAllotedSpace().x * relativeLeft);
		newLocation.y = OriginalRenderGeometry.GetLocation().y + (OriginalRenderGeometry.GetAllotedSpace().y * relativeBottom);

		newSpace.x = (relativeRight - relativeLeft) * OriginalRenderGeometry.GetAllotedSpace().x;
		newSpace.y = (relativeTop - relativeBottom) * OriginalRenderGeometry.GetAllotedSpace().y;

		OutRenderGeometry.SetLocation(newLocation);
		OutRenderGeometry.SetAllotedSpace(newSpace);
	}
	float& operator[](const int32 Index) {
		return Sides[Index];
	}
protected:
	float Sides[4];
	bool Relative[4] = { true };

	float GetSideRelativeValue(const EPictorumSide& Side, const vec2& RenderResolution) const {
		if (Relative[(uint8)Side]) {
			return Sides[(uint8)Side];
		} else {
			if (Side == EPictorumSide::BOTTOM || Side == EPictorumSide::TOP) {
				return Sides[(uint8)Side] / RenderResolution.y;
			} else {
				return Sides[(uint8)Side] / RenderResolution.x;
			}
		}
		return 0.0f;
	}
};
struct FPadding : public FAnchors{
	FPadding() : FAnchors() {
		memset(&Sides[0], 0, 4);
	}
	void ApplyToGeometry(const FRenderGeometry& OriginalRenderGeometry, FRenderGeometry& OutRenderGeometry) const override {
		float relativeTop    = GetSideRelativeValue(EPictorumSide::TOP, OriginalRenderGeometry.GetRenderResolution());
		float relativeRight  = GetSideRelativeValue(EPictorumSide::RIGHT, OriginalRenderGeometry.GetRenderResolution());
		float relativeBottom = GetSideRelativeValue(EPictorumSide::BOTTOM, OriginalRenderGeometry.GetRenderResolution());
		float relativeLeft   = GetSideRelativeValue(EPictorumSide::LEFT, OriginalRenderGeometry.GetRenderResolution());

		OutRenderGeometry.AddLocation(relativeLeft, relativeBottom);
		OutRenderGeometry.AddAllotedSpace(-relativeRight * 2, -relativeTop * 2);
	}
};
struct FMargins : public FAnchors{
	FMargins() : FAnchors() {
		memset(&Sides[0], 0, 4);
	}
	void ApplyToGeometry(const FRenderGeometry& OriginalRenderGeometry, FRenderGeometry& OutRenderGeometry) const override {
		float relativeTop    = GetSideRelativeValue(EPictorumSide::TOP, OriginalRenderGeometry.GetRenderResolution());
		float relativeRight  = GetSideRelativeValue(EPictorumSide::RIGHT, OriginalRenderGeometry.GetRenderResolution());
		float relativeBottom = GetSideRelativeValue(EPictorumSide::BOTTOM, OriginalRenderGeometry.GetRenderResolution());
		float relativeLeft   = GetSideRelativeValue(EPictorumSide::LEFT, OriginalRenderGeometry.GetRenderResolution());

		OutRenderGeometry.AddLocation(relativeLeft, relativeBottom);
		OutRenderGeometry.AddAllotedSpace(-relativeRight * 2, -relativeTop * 2);
	}
};
