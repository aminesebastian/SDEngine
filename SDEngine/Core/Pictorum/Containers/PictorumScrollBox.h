#pragma once
#include "Core/Pictorum/PictorumWidget.h"

class PictorumScrollBox : public PictorumWidget {
public:
	PictorumScrollBox(const TString& Name);
	virtual ~PictorumScrollBox();
	virtual void Tick(float DeltaTime, const FRenderGeometry& Geometry) override;
	virtual void CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const override;
	virtual vec2 GetDesiredDrawSpace(const FRenderGeometry& Geometry) const override;

	void AddScrollPosition(const float& ScrollAmount);
	void SetScrollPosition(const float& ScrollPosition);
	const float& GetScrollPosition() const;
	const float GetScrollAlpha() const;

	void SetSelectionEnabled(const bool& Enabled);
	const bool& IsSelectionEnabled() const;
	const bool ShouldRenderScrollBar() const;

	Event<void(PictorumWidget*, const SArray<int32>&)> SelectionUpdated;

protected:
	bool bSelectionEnabled;
	bool bWasRightClickedIn;
	bool bIsScrollingUsingScrollBar;

	FColor ScrollBarColor;
	FColor ScrollBarHoveredColor;

	float MinScrollBarHeight;
	float ScrollBarThickness;
	float ScrollBarSideOffset;
	float ScrollSpeed;
	float ScrollBarPadding;
	
	float ScrollOffset;
	float ScrollEnergy;
	float ScrollDampening;

	FBoxDrawInstruction* ScrollBarDrawInstruction;
	FBoxDrawInstruction* ScrollBarLineDrawInstruction;
	FBoxDrawInstruction* EntryBackgroundDrawInstruction;

	int32 HoveredIndex;
	vec2 LastMouseDelta;
	SArray<int32> SelectedIndices;

	virtual void Draw(float DeltaTime, const FRenderGeometry& Geometry) override;
	virtual void OnDrawCompleted(const float& DeltaTime, const FRenderGeometry& Geometry) override;

	const float GetOffsetForChild(const int32& ChildIndex) const;
	const vec2 CalculateSizeOfContents(const FRenderGeometry& Geometry) const;

	virtual void OnChildAdded(PictorumWidget* Widget) override;
	virtual void OnChildRemoved(PictorumWidget* Widget) override;
	virtual void OnMouseMove(const vec2& MousePosition, const vec2& MouseDelta, FUserInterfaceEvent& EventIn) override;
	virtual void OnMouseDown(const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& EventIn) override;
	virtual void OnMouseUp(const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& EventIn) override;
	virtual void OnMouseScroll(const float Delta, FUserInterfaceEvent& EventIn) override;

	void OnScrollEntryHovered(PictorumWidget* Widget, const vec2& MouseLocation, FUserInterfaceEvent& EventIn);
	void OnScrollEntryUnhovered(PictorumWidget* Widget, const vec2& MouseLocation, FUserInterfaceEvent& EventIn);
	void OnScrollEntryClicked(PictorumWidget* Widget, const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& EventIn);
	void PreventOverscroll();
	void DrawEntryBackground(const FRenderGeometry& Geometry, const int32& ChildIndex, const FColor& Color);
};

