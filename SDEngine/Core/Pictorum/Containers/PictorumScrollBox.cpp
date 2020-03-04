#include "PictorumScrollBox.h"

PictorumScrollBox::PictorumScrollBox(const TString& Name) : PictorumWidget(Name) {
	ScrollOffset               = 0.0f;
	ScrollSpeed                = 15.0f;
	bWasRightClickedIn         = false;
	bIsScrollingUsingScrollBar = false;
	bSelectionEnabled          = true;
	ScrollDampening            = 150.0f;
	ScrollEnergy               = 0.0f;
	ScrollBarSideOffset        = 10.0f;
	MinScrollBarHeight         = 15.0f;
	ScrollBarThickness         = 5.0f;
	ScrollBarPadding           = 5.0f;
	HoveredIndex               = -1;
	ScrollBarColor             = FColor(0.5f, 0.5f, 0.5f);
	ScrollBarHoveredColor      = FColor(0.65f, 0.65f, 0.654f);

	ScrollBarDrawInstruction = new FBoxDrawInstruction();
	ScrollBarDrawInstruction->BackgroundColor = ScrollBarColor;
	ScrollBarDrawInstruction->BorderRadius.SetAllRadii(4.0f);

	ScrollBarLineDrawInstruction = new FBoxDrawInstruction();
	ScrollBarLineDrawInstruction->BackgroundColor = FColor(0.2f, 0.2f, 0.2f);
	ScrollBarLineDrawInstruction->Size.x = 1.0f;

	EntryBackgroundDrawInstruction = new FBoxDrawInstruction();

	SetVisibility(EPictorumVisibilityState::VISIBLE);
}
PictorumScrollBox::~PictorumScrollBox() {

}
void PictorumScrollBox::Draw(float DeltaTime, const FRenderGeometry& Geometry) {
	// Draw the selection and hovered highlights if selection is enabled.
	if (bSelectionEnabled) {
		if (HoveredIndex >= 0) {
			DrawEntryBackground(Geometry, HoveredIndex, FColor(0.2f, 0.2f, 0.3f, 0.5f));
		}
		for (const int32& selectedIndex : SelectedIndices) {
			DrawEntryBackground(Geometry, selectedIndex, FColor(0.4f, 0.4f, 0.5f, 0.5f));
		}
	}
}
void PictorumScrollBox::PostChildrenDraw(float DeltaTime, const FRenderGeometry& Geometry) {
	if (ShouldRenderScrollBar()) {
		// Draw the line behind the scroll bar.
		ScrollBarLineDrawInstruction->Size.y = Geometry.GetAllotedSpace().y - 10.0f;
		ScrollBarLineDrawInstruction->Location = Geometry.GetLocation();
		ScrollBarLineDrawInstruction->Location.y += 5.0f;
		ScrollBarLineDrawInstruction->Location.x += Geometry.GetAllotedSpace().x;
		ScrollBarLineDrawInstruction->Location.x -= (ScrollBarSideOffset + (ScrollBarLineDrawInstruction->Size.x / 2.0f) - (ScrollBarThickness / 2.0f));
		DrawBox(Geometry, *ScrollBarLineDrawInstruction);

		// Draw the scroll bar.
		float scrollBarHeight = Geometry.GetAllotedSpace().y * (Geometry.GetAllotedSpace().y / CalculateSizeOfContents(Geometry).y);
		scrollBarHeight = MathLibrary::Max(MinScrollBarHeight, scrollBarHeight);
		ScrollBarDrawInstruction->Location = Geometry.GetLocation();
		ScrollBarDrawInstruction->Location.y += ScrollBarPadding;
		ScrollBarDrawInstruction->Location.x += Geometry.GetAllotedSpace().x - ScrollBarSideOffset;
		ScrollBarDrawInstruction->Location.y += Geometry.GetAllotedSpace().y - scrollBarHeight - ((Geometry.GetAllotedSpace().y - scrollBarHeight) * GetScrollAlpha());
		ScrollBarDrawInstruction->Size = vec2(ScrollBarThickness, scrollBarHeight);
		ScrollBarDrawInstruction->Size.y -= (2 * ScrollBarPadding);
		DrawBox(Geometry, *ScrollBarDrawInstruction);
	}
}
void PictorumScrollBox::Tick(float DeltaTime, const FRenderGeometry& Geometry) {
	if (abs(ScrollEnergy) > 0.0f) {
		ScrollOffset += ScrollEnergy * DeltaTime;
		if (ScrollEnergy < 0.0f) {
			ScrollEnergy += MathLibrary::Min(-ScrollEnergy, ScrollDampening * DeltaTime);
		} else if (ScrollEnergy > 0.0f) {
			ScrollEnergy -= MathLibrary::Min(ScrollEnergy, ScrollDampening * DeltaTime);
		}
		PreventOverscroll();
	}
	if (ScrollOffset > CalculateSizeOfContents(Geometry).y - Geometry.GetAllotedSpace().y) {
		ScrollOffset = MathLibrary::Max(0.0f, CalculateSizeOfContents(Geometry).y - Geometry.GetAllotedSpace().y);
	}
}
void PictorumScrollBox::CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const {
	float offset = GetOffsetForChild(ChildIndex);

	vec2 desiredSize = Children[ChildIndex]->GetDesiredDrawSpace(CurrentRenderGeometry);

	OutputGeometry.AddLocation(0.0f, CurrentRenderGeometry.GetAllotedSpace().y);
	OutputGeometry.AddLocation(0.0f, -offset);
	OutputGeometry.AddLocation(0.0f, ScrollOffset);
	OutputGeometry.AddLocation(0.0f, -desiredSize.y);
	OutputGeometry.SetAllotedSpace(vec2(CurrentRenderGeometry.GetAllotedSpace().x, desiredSize.y));

	OutputGeometry.SetMinimumClipPoint(CurrentRenderGeometry.GetLocation());
	OutputGeometry.SetMaximumClipPoint(CurrentRenderGeometry.GetAllotedSpace());
}
vec2 PictorumScrollBox::GetDesiredDrawSpace(const FRenderGeometry& Geometry) const {
	// Return a size of 0 as this widget does not care about what size it is drawn at.
	vec2 size = vec2(0.0f, 0.0f);
	return size;
}
const vec2 PictorumScrollBox::CalculateSizeOfContents(const FRenderGeometry& Geometry) const {
	vec2 size = vec2(0.0f, 0.0f);

	for (PictorumWidget* child : Children) {
		vec2 desired = child->GetDesiredDrawSpace(Geometry);
		size.x = MathLibrary::Max(desired.x, size.x);
		size.y += desired.y;
	}
	return size;
}

void PictorumScrollBox::AddScrollPosition(const float& ScrollAmount) {
	ScrollOffset = ScrollOffset + ScrollAmount;
	PreventOverscroll();
}
void PictorumScrollBox::SetScrollPosition(const float& ScrollPosition) {
	ScrollOffset = ScrollPosition;
	PreventOverscroll();
}

void PictorumScrollBox::OnChildAdded(PictorumWidget* Widget) {
	Widget->OnHoveredDelegate.Add<PictorumScrollBox, & PictorumScrollBox::OnScrollEntryHovered>(this);
	Widget->OnUnhoveredDelegate.Add<PictorumScrollBox, & PictorumScrollBox::OnScrollEntryUnhovered>(this);
	Widget->OnMouseUpDelegate.Add<PictorumScrollBox, & PictorumScrollBox::OnScrollEntryClicked>(this);
}
void PictorumScrollBox::OnChildRemoved(PictorumWidget* Widget) {
	//Widget->OnHoveredDelegate.Remove<PictorumScrollBox, &PictorumScrollBox::OnScrollEntryHovered>(this);
	//Widget->OnUnhoveredDelegate.Remove<PictorumScrollBox, &PictorumScrollBox::OnScrollEntryUnhovered>(this);
}
void PictorumScrollBox::OnMouseMove(const vec2& MousePosition, const vec2& MouseDelta, FUserInterfaceEvent& EventIn) {
	if (bWasRightClickedIn) {
		AddScrollPosition(MouseDelta.y);
		LastMouseDelta = MouseDelta;
	} else if (bIsScrollingUsingScrollBar) {
		AddScrollPosition(-MouseDelta.y); // Improve the tracking here.
	}
	if (MathLibrary::PointIntersect2DBox(MousePosition, ScrollBarDrawInstruction->Location, ScrollBarDrawInstruction->Location + ScrollBarDrawInstruction->Size)) {
		ScrollBarDrawInstruction->BackgroundColor = ScrollBarHoveredColor;
	} else {
		ScrollBarDrawInstruction->BackgroundColor = ScrollBarColor;
	}
}
void PictorumScrollBox::OnMouseDown(const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& EventIn) {
	if (Button == EMouseButton::RIGHT) {
		bWasRightClickedIn = true;
	} else if (Button == EMouseButton::LEFT) {
		if (MathLibrary::PointIntersect2DBox(MousePosition, ScrollBarDrawInstruction->Location, ScrollBarDrawInstruction->Location + ScrollBarDrawInstruction->Size)) {
			ScrollBarDrawInstruction->BackgroundColor = ScrollBarHoveredColor;
			bIsScrollingUsingScrollBar = true;
		}
	}
}
void PictorumScrollBox::OnMouseUp(const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& EventIn) {
	if (bWasRightClickedIn) {
		ScrollEnergy = LastMouseDelta.y * 10.0f;
		bWasRightClickedIn = false;
	}
	if (bIsScrollingUsingScrollBar) {
		bIsScrollingUsingScrollBar = false;
	}
}
void PictorumScrollBox::OnMouseScroll(const float Delta, FUserInterfaceEvent& EventIn) {
	AddScrollPosition(-Delta * ScrollSpeed);
}
void PictorumScrollBox::OnScrollEntryHovered(PictorumWidget* Widget, const vec2& MouseLocation, FUserInterfaceEvent& EventIn) {
	HoveredIndex = GetIndexOfChild(Widget);
}
void PictorumScrollBox::OnScrollEntryUnhovered(PictorumWidget* Widget, const vec2& MouseLocation, FUserInterfaceEvent& EventIn) {
	HoveredIndex = -1;
}
void PictorumScrollBox::OnScrollEntryClicked(PictorumWidget* Widget, const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& EventIn) {
	SelectedIndices.Clear();
	SelectedIndices.Add(GetIndexOfChild(Widget));
	SelectionUpdated.Broadcast(this, SelectedIndices);
}

const float PictorumScrollBox::GetOffsetForChild(const int32& ChildIndex) const {
	float offset = 0.0f;
	for (int i = 0; i < MathLibrary::Min(ChildIndex, Children.Count()); i++) {
		offset += Children[i]->GetDesiredDrawSpace(LastRenderedGeometry).y;
	}
	return offset;
}
const float PictorumScrollBox::GetScrollAlpha() const {
	return ScrollOffset / MathLibrary::Max(0.00001f, CalculateSizeOfContents(LastRenderedGeometry).y - LastRenderedGeometry.GetAllotedSpace().y); // Cover for the case where the desired is 0. NO DIVIDE BY ZERO HERE.
}
const float& PictorumScrollBox::GetScrollPosition() const {
	return ScrollOffset;
}
void PictorumScrollBox::SetSelectionEnabled(const bool& Enabled) {
	bSelectionEnabled = Enabled;
}
const bool& PictorumScrollBox::IsSelectionEnabled() const {
	return bSelectionEnabled;
}
const bool PictorumScrollBox::ShouldRenderScrollBar() const {
	return CalculateSizeOfContents(LastRenderedGeometry).y > LastRenderedGeometry.GetAllotedSpace().y;
}
void PictorumScrollBox::PreventOverscroll() {
	if (ScrollOffset > CalculateSizeOfContents(LastRenderedGeometry).y - LastRenderedGeometry.GetAllotedSpace().y) {
		ScrollOffset = CalculateSizeOfContents(LastRenderedGeometry).y - LastRenderedGeometry.GetAllotedSpace().y;
	}
	if (ScrollOffset < 0.0f) {
		ScrollOffset = 0.0f;
	}
}
void PictorumScrollBox::DrawEntryBackground(const FRenderGeometry& Geometry, const int32& ChildIndex, const FColor& Color) {
	float offset = GetOffsetForChild(ChildIndex);
	PictorumWidget* hovered = Children[ChildIndex];
	Vector2D desiredSize = hovered->GetDesiredDrawSpace(Geometry);

	EntryBackgroundDrawInstruction->Location = Geometry.GetLocation();
	EntryBackgroundDrawInstruction->Location.y += Geometry.GetAllotedSpace().y;
	EntryBackgroundDrawInstruction->Location.y -= (offset - ScrollOffset);
	EntryBackgroundDrawInstruction->Location.y -= desiredSize.y;
	EntryBackgroundDrawInstruction->Size = desiredSize;
	EntryBackgroundDrawInstruction->Size.x = Geometry.GetAllotedSpace().x;
	EntryBackgroundDrawInstruction->BackgroundColor = Color;
	DrawBox(Geometry, *EntryBackgroundDrawInstruction);
}