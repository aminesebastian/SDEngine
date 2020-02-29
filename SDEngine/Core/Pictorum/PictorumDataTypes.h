#pragma once
#include "Core/DataStructures/Array.h"
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Reflection/Reflection.h"
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
	void Handled() {
		bHandled = true;
	}
	void CaptureMouse() {
		Handled();
		bCaptureMouse = true;
	}
	bool ShouldContinuePropragating() {
		return !bHandled;
	}
private:
	friend class PictorumRenderer;

	bool bHandled;
	bool bCaptureMouse;
};
SD_STRUCT()
struct FFillRule {
	SD_STRUCT_BODY();

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
SD_STRUCT()
struct FRenderGeometry {
	SD_STRUCT_BODY();

	FRenderGeometry() {
		AllotedSpace = vec2(0.0f, 0.0f);
		Location = vec2(0.0f, 0.0f);
		MinClipPoint = vec2(0.0f, 0.0f);
		MaxClipPoint = vec2(0.0f, 0.0f);
	}
	explicit FRenderGeometry(const vec2& RenderResolution, const vec2& RenderTargetDPI) : RenderResolution(RenderResolution), RenderTargetDPI(RenderTargetDPI) {
		MinClipPoint = vec2(0.0f, 0.0f);
		MaxClipPoint = RenderResolution;
	}

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
		} else if (Basis == EPictorumLocationBasis::RELATIVE) {
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
	 * Sets minimum clip point. Any point to the Left or Below this point will be clipped if clipping
	 * is enabled.
	 *
	 * @param 	MinimumClip	The minimum clip point.
	 */
	void SetMinimumClipPoint(const vec2& MinimumClip) {
		MinClipPoint = MinimumClip;
	}
	/**
	 * Gets minimum clip point. Any point to the Left or Below this point will be clipped if
	 * clipping is enabled.
	 *
	 * @param 	Basis	(Optional) The basis.
	 *
	 * @returns	The minimum clip point.
	 */
	const vec2 GetMinimumClipPoint(EPictorumLocationBasis Basis = EPictorumLocationBasis::ABSOLUTE) const {
		if (Basis == EPictorumLocationBasis::ABSOLUTE) {
			return MinClipPoint;
		} else {
			return MinClipPoint / RenderResolution;
		}
	}
	/**
	 * Sets maximum clip point. Any point to the Right or Above this point will be clipped if clipping
	 * is enabled.
	 *
	 * @param 	MaximumClip	The maximum clip point.
	 */
	void SetMaximumClipPoint(const vec2& MaximumClip) {
		MaxClipPoint = MaximumClip;
	}
	/**
	 * Gets maximum clip point. Any point to the Right or Above this point will be clipped if
	 * clipping is enabled.
	 *
	 * @param 	Basis	(Optional) The basis.
	 *
	 * @returns	The maximum clip point.
	 */
	const vec2 GetMaximumClipPoint(EPictorumLocationBasis Basis = EPictorumLocationBasis::ABSOLUTE) const {
		if (Basis == EPictorumLocationBasis::ABSOLUTE) {
			return MaxClipPoint;
		} else {
			return MaxClipPoint / RenderResolution;
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
	/** Any point to the Left or Bottom of the line should be clipped. */
	vec2 MinClipPoint;
	/** Any point to the Right or Top of this line should be clipped. */
	vec2 MaxClipPoint;
};
SD_STRUCT()
struct FAnchors {
	SD_STRUCT_BODY();

	FAnchors() {
		Sides.Add(0.0f);
		Sides.Add(0.0f);
		Sides.Add(0.0f);
		Sides.Add(0.0f);
		SetTop(1.0f);
		SetRight(1.0f);
		SetBottom(0.0f);
		SetLeft(0.0f);
	}
	const float& GetTop() const {
		return Sides[0];
	}
	FAnchors& SetTop(const float& Value) {
		Sides[0] = Value;
		return *this;
	}
	const float& GetRight() const {
		return Sides[1];
	}
	FAnchors& SetRight(const float& Value) {
		Sides[1] = Value;
		return *this;
	}
	const float& GetBottom() const {
		return Sides[2];
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
	float& operator[](const int32 Index) {
		return Sides[Index];
	}

protected:
	SArray<float> Sides;
};
SD_STRUCT()
struct FOffsets {
	SD_STRUCT_BODY();

	FOffsets() {
		Sides.Resize(4);
		Relative.Resize(4, false);
		PivotOffset = vec2(0.0f, 0.0f);
	}

	const float& GetYSize() const {
		return GetTop();
	}
	FOffsets& SetYSize(const float& Value) {
		SetTop(Value);
		return *this;
	}
	const float& GetTop() const {
		return Sides[0];
	}
	FOffsets& SetTop(const float& Value) {
		Sides[0] = Value;
		return *this;
	}
	const float& GetXSize() const {
		return GetRight();
	}
	FOffsets& SetXSize(const float& Value) {
		SetRight(Value);
		return *this;
	}
	const float& GetRight() const {
		return Sides[1];
	}
	FOffsets& SetRight(const float& Value) {
		Sides[1] = Value;
		return *this;
	}
	const float& GetYPosition() const {
		return GetBottom();
	}
	FOffsets& SetYPosition(const float& Value) {
		SetBottom(Value);
		return *this;
	}
	const float& GetBottom() const {
		return Sides[2];
	}
	FOffsets& SetBottom(const float& Value) {
		Sides[2] = Value;
		return *this;
	}
	const float& GetXPosition() const {
		return GetLeft();
	}
	FOffsets& SetXPosition(const float& Value) {
		SetLeft(Value);
		return *this;
	}
	const float& GetLeft() const {
		return Sides[3];
	}
	FOffsets& SetLeft(const float& Value) {
		Sides[3] = Value;
		return *this;
	}
	FOffsets& SetXPivotOffset(const float& X) {
		PivotOffset.x = X;
		return *this;
	}
	FOffsets& SetYPivotOffset(const float& Y) {
		PivotOffset.y = Y;
		return *this;
	}
	FOffsets& SetPivotOffset(const float& X, const float& Y) {
		PivotOffset.x = X;
		PivotOffset.y = Y;
		return *this;
	}
	const Vector2D GetPivotOffset() const {
		return PivotOffset;
	}
	float& operator[](const int32 Index) {
		return Sides[Index];
	}
protected:
	SArray<float> Sides;
	SArray<bool> Relative;
	vec2 PivotOffset;

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
SD_STRUCT()
struct FPadding {
	SD_STRUCT_BODY();

	FPadding() {
		Sides.Resize(4);
	}
	const float& GetTop() const {
		return Sides[0];
	}
	FPadding& SetTop(const float& Value) {
		Sides[0] = Value;
		return *this;
	}
	const float& GetRight() const {
		return Sides[1];
	}
	FPadding& SetRight(const float& Value) {
		Sides[1] = Value;
		return *this;
	}
	const float& GetBottom() const {
		return Sides[2];
	}
	FPadding& SetBottom(const float& Value) {
		Sides[2] = Value;
		return *this;
	}
	const float& GetLeft() const {
		return Sides[3];
	}
	FPadding& SetLeft(const float& Value) {
		Sides[3] = Value;
		return *this;
	}
	float& operator[](const int32 Index) {
		return Sides[Index];
	}
protected:
	SArray<float> Sides;

	float GetSideRelativeValue(const EPictorumSide& Side, const vec2& RenderResolution) const {
		if (Side == EPictorumSide::BOTTOM || Side == EPictorumSide::TOP) {
			return Sides[(uint8)Side] / RenderResolution.y;
		} else {
			return Sides[(uint8)Side] / RenderResolution.x;
		}
	}
};
SD_STRUCT()
struct FMargins {
	SD_STRUCT_BODY();

	FMargins() {
		Sides.Resize(4);
	}
	const float& GetTop() const {
		return Sides[0];
	}
	FMargins& SetTop(const float& Value) {
		Sides[0] = Value;
		return *this;
	}
	const float& GetRight() const {
		return Sides[1];
	}
	FMargins& SetRight(const float& Value) {
		Sides[1] = Value;
		return *this;
	}
	const float& GetBottom() const {
		return Sides[2];
	}
	FMargins& SetBottom(const float& Value) {
		Sides[2] = Value;
		return *this;
	}
	const float& GetLeft() const {
		return Sides[3];
	}
	FMargins& SetLeft(const float& Value) {
		Sides[3] = Value;
		return *this;
	}
	float& operator[](const int32 Index) {
		return Sides[Index];
	}
protected:
	SArray<float> Sides;

	float GetSideRelativeValue(const EPictorumSide& Side, const vec2& RenderResolution) const {
		if (Side == EPictorumSide::BOTTOM || Side == EPictorumSide::TOP) {
			return Sides[(uint8)Side] / RenderResolution.y;
		} else {
			return Sides[(uint8)Side] / RenderResolution.x;
		}
	}
};
SD_STRUCT()
struct FBorderRadius {
	SD_STRUCT_BODY();

	FBorderRadius() {
		Radii = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}
	FBorderRadius(const float& Radius) {
		Radii = vec4(Radius, Radius, Radius, Radius);
	}
	void SetAllRadii(const float& Radius) {
		Radii.x = Radius;
		Radii.y = Radius;
		Radii.z = Radius;
		Radii.w = Radius;
	}
	const Vector4D& GetRadii() const {
		return Radii;
	}
	const float& GetTopLeftRadius() const {
		return Radii.w;
	}
	void SetTopLeftRadius(const float& Radius) {
		Radii.w = Radius;
	}
	const float& GetTopRightRadius() const {
		return Radii.x;
	}
	void SetTopRightRadius(const float& Radius) {
		Radii.x = Radius;
	}
	const float& GetBottomLeftRadius() const {
		return Radii.z;
	}
	void SetBottomLeftRadius(const float& Radius) {
		Radii.z = Radius;
	}
	const float& GetBottomRightRadius() const {
		return Radii.y;
	}
	void SetBottomRightRadius(const float& Radius) {
		Radii.y = Radius;
	}

private:
	Vector4D Radii;
};

