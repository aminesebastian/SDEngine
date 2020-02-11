#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Utilities/Logger.h"

enum class EVerticalAlignment : uint8 {
	TOP,
	CENTER,
	BOTTOM,
	STRETCH
};
enum class EHorizontalAlignment : uint8 {
	LEFT,
	CENTER,
	RIGHT,
	STRETCH
};
enum class EFillRule : uint8 {
	AUTOMATIC,
	FILL
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
enum class ETextAlignment : uint8 {
	LEFT,
	CENTER,
	RIGHT,
	JUSTIFIED
};
/** The font weight to use when rendering text. */
enum class EFontWeight : uint8 {
	Light,
	Normal,
	Bold
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
struct FFillRule {
	FFillRule() {
		Ratio = 1.0f;
		Rule = EFillRule::AUTOMATIC;
	}
	void SetFillAllSpace(const float& FillRatio) {
		if (FillRatio > 0.0f) {
			Ratio = FillRatio;
		} else {
			Ratio = 0.0000001f;
			SD_ENGINE_ERROR("There was an attempt to set the fill ratio of a fill rule to 0.");
		}
		Rule = EFillRule::FILL;
	}
	void SetUseWidgetDesiredSpace() {
		Ratio = 0.0f;
		Rule = EFillRule::AUTOMATIC;
	}
	const float& GetFillRatio() const {
		return Ratio;
	}
	const EFillRule& GetFillRule() const {
		return Rule;
	}
private:
	float Ratio;
	EFillRule Rule;
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
	void SetLocation(vec2 LocationIn, EPictorumLocationBasis Basis = EPictorumLocationBasis::ABSOLUTE) {
		if (Basis == EPictorumLocationBasis::ABSOLUTE) {
			Location = LocationIn;
		} else {
			Location = LocationIn * RenderResolution;
		}
	}
	/**
	* Adds an offset to the location.
	*
	* @param 	{float}	X						 	The X offset.
	* @param 	{float}	Y						 	The Y offset.
	* @param 	{EPictorumLocationBasis}	Basis	(Optional) The basis the offsets are supplied in.
	*/
	void AddLocation(float X, float Y, EPictorumLocationBasis Basis = EPictorumLocationBasis::ABSOLUTE) {
		if (Basis == EPictorumLocationBasis::ABSOLUTE) {
			Location.x += X;
			Location.y += Y;
		} else if(Basis == EPictorumLocationBasis::RELATIVE) {
			Location.x += (X * RenderResolution.x);
			Location.y += (Y * RenderResolution.y);
		}
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
	void SetAllotedSpace(vec2 Space, EPictorumScaleBasis Basis = EPictorumScaleBasis::ABSOLUTE) {
		if (Basis == EPictorumScaleBasis::ABSOLUTE) {
			AllotedSpace = Space;
		} else {
			AllotedSpace = Space * RenderResolution;
		}
	}
	/**
	* Adds an offset to the alloted space.
	*
	* @param 	{float}	X						 	The X offset.
	* @param 	{float}	Y						 	The Y offset.
	* @param 	{EPictorumScaleBasis}	Basis	(Optional) The basis the offsets are supplied in.
	*/
	void AddAllotedSpace(float X, float Y, EPictorumScaleBasis Basis = EPictorumScaleBasis::ABSOLUTE) {
		if (Basis == EPictorumScaleBasis::ABSOLUTE) {
			AllotedSpace.x += X;
			AllotedSpace.y += Y;
		} else {
			AllotedSpace.x += (X * RenderResolution.x);
			AllotedSpace.y += (Y * RenderResolution.y);
		}
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
struct FOffsets {
	FOffsets() {
		memset(&Sides[0], 0, sizeof(float) * 4);
		memset(&Relative[0], 0, sizeof(uint8) * 4);
	}
	FOffsets& SetSide(const EPictorumSide& Side, const float& Value) {
		Sides[(uint8)Side] = Value;
		return *this;
	}
	const float& GetTop() const {
		return Sides[0];
	}
	FOffsets& SetTop(const float& Value) {
		Sides[0] = Value;
		return *this;
	}
	const float& GetRight() const {
		return Sides[1];
	}
	FOffsets& SetRight(const float& Value) {
		Sides[1] = Value;
		return *this;
	}
	const float& GetBottom() const {
		return Sides[2];
	}
	FOffsets& SetBottom(const float& Value) {
		Sides[2] = Value;
		return *this;
	}
	const float& GetLeft() const {
		return Sides[3];
	}
	FOffsets& SetLeft(const float& Value) {
		Sides[3] = Value;
		return *this;
	}
	virtual void ApplyToGeometry(const FRenderGeometry& OriginalRenderGeometry, FRenderGeometry& OutRenderGeometry) const {
		vec2 originalLocation = OriginalRenderGeometry.GetLocation(EPictorumLocationBasis::RELATIVE);
		vec2 originalSpace    = OriginalRenderGeometry.GetAllotedSpace(EPictorumScaleBasis::RELATIVE);
		float relativeTop     = GetSideRelativeValue(EPictorumSide::TOP, OriginalRenderGeometry.GetRenderResolution());
		float relativeRight   = GetSideRelativeValue(EPictorumSide::RIGHT, OriginalRenderGeometry.GetRenderResolution());
		float relativeBottom  = GetSideRelativeValue(EPictorumSide::BOTTOM, OriginalRenderGeometry.GetRenderResolution());
		float relativeLeft    = GetSideRelativeValue(EPictorumSide::LEFT, OriginalRenderGeometry.GetRenderResolution());

		vec2 newLocation = originalLocation + vec2(relativeLeft, relativeBottom);
		vec2 newSpace = originalSpace + vec2(relativeRight - relativeLeft, relativeTop - relativeBottom);

		OutRenderGeometry.SetLocation(newLocation, EPictorumLocationBasis::RELATIVE);
		OutRenderGeometry.SetAllotedSpace(newSpace, EPictorumScaleBasis::RELATIVE);
	}
	float& operator[](const int32 Index) {
		return Sides[Index];
	}
protected:
	float Sides[4];
	bool Relative[4] = { true };

	float GetSideRelativeValue(const EPictorumSide& Side, const vec2& RenderResolution) const {
		float value = Sides[(uint8)Side];
		if (Relative[(uint8)Side] == true) {
			return value;
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
struct FPadding : public FOffsets {
	FPadding() : FOffsets() {
		memset(&Sides[0], 0, sizeof(float) * 4);
		memset(&Relative[0], 0, sizeof(bool) * 4);
	}

	void ApplyToGeometry(const FRenderGeometry& OriginalRenderGeometry, FRenderGeometry& OutRenderGeometry) const override {
		float relativeTop    = GetSideRelativeValue(EPictorumSide::TOP, OriginalRenderGeometry.GetRenderResolution());
		float relativeRight  = GetSideRelativeValue(EPictorumSide::RIGHT, OriginalRenderGeometry.GetRenderResolution());
		float relativeBottom = GetSideRelativeValue(EPictorumSide::BOTTOM, OriginalRenderGeometry.GetRenderResolution());
		float relativeLeft   = GetSideRelativeValue(EPictorumSide::LEFT, OriginalRenderGeometry.GetRenderResolution());

		OutRenderGeometry.AddLocation(relativeLeft, relativeBottom, EPictorumLocationBasis::RELATIVE);
		OutRenderGeometry.AddAllotedSpace(-relativeRight, -relativeTop, EPictorumScaleBasis::RELATIVE);
	}
};
struct FMargins : public FOffsets {
	FMargins() : FOffsets() {
		memset(&Sides[0], 0, sizeof(float) * 4);
		memset(&Relative[0], 0, sizeof(bool) * 4);
	}

	void ApplyToGeometry(const FRenderGeometry& OriginalRenderGeometry, FRenderGeometry& OutRenderGeometry) const override {
		float relativeTop    = GetSideRelativeValue(EPictorumSide::TOP, OriginalRenderGeometry.GetRenderResolution());
		float relativeRight  = GetSideRelativeValue(EPictorumSide::RIGHT, OriginalRenderGeometry.GetRenderResolution());
		float relativeBottom = GetSideRelativeValue(EPictorumSide::BOTTOM, OriginalRenderGeometry.GetRenderResolution());
		float relativeLeft   = GetSideRelativeValue(EPictorumSide::LEFT, OriginalRenderGeometry.GetRenderResolution());

		OutRenderGeometry.AddLocation(relativeLeft, relativeBottom, EPictorumLocationBasis::RELATIVE);
		OutRenderGeometry.AddAllotedSpace(-relativeRight, -relativeTop, EPictorumScaleBasis::RELATIVE);
	}
};
struct FAnchors : public FOffsets {
	FAnchors() : FOffsets() {
		SetTop(1.0f);
		SetRight(1.0f);
		SetBottom(0.0f);
		SetLeft(0.0f);
		memset(&Relative[0], 1, sizeof(bool) * 4);
	}

	void ApplyToGeometry(const FRenderGeometry& OriginalRenderGeometry, FRenderGeometry& OutRenderGeometry) const override {
		vec2 originalLocation = OriginalRenderGeometry.GetLocation(EPictorumLocationBasis::RELATIVE);
		vec2 originalSpace    = OriginalRenderGeometry.GetAllotedSpace(EPictorumScaleBasis::RELATIVE);
		float relativeTop     = GetSideRelativeValue(EPictorumSide::TOP, OriginalRenderGeometry.GetRenderResolution());
		float relativeRight   = GetSideRelativeValue(EPictorumSide::RIGHT, OriginalRenderGeometry.GetRenderResolution());
		float relativeBottom  = GetSideRelativeValue(EPictorumSide::BOTTOM, OriginalRenderGeometry.GetRenderResolution());
		float relativeLeft    = GetSideRelativeValue(EPictorumSide::LEFT, OriginalRenderGeometry.GetRenderResolution());

		vec2 newLocation = originalLocation + vec2(relativeLeft, relativeBottom);
		vec2 newSpace = vec2(relativeRight - relativeLeft, relativeTop - relativeBottom);

		OutRenderGeometry.SetLocation(newLocation, EPictorumLocationBasis::RELATIVE);
		OutRenderGeometry.SetAllotedSpace(newSpace, EPictorumScaleBasis::RELATIVE);
	}
};
