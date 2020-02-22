#include "PictorumScrollBox.h"

PictorumScrollBox::PictorumScrollBox(const TString& Name) : PictorumWidget(Name) {
	ScrollOffset = 0.0f;
	ScrollSpeed = 10.0f;
	bWasRightClickedIn = false;
	ScrollDampening = 150.0f;
	ScrollEnergy = 0.0f;
	SetVisibility(EPictorumVisibilityState::VISIBLE);
}
PictorumScrollBox::~PictorumScrollBox() {

}

void PictorumScrollBox::Draw(float DeltaTime, const FRenderGeometry& Geometry) {
	FBoxDrawInstruction instructions;
	instructions.BackgroundColor = FColor(0.5f, 0.5f, 0.5f);
	instructions.Location = Geometry.GetLocation();
	instructions.Location.x += Geometry.GetAllotedSpace().x - 15.0f;
	instructions.Location.y += Geometry.GetAllotedSpace().y - ((Geometry.GetAllotedSpace().y + 50.0f) * GetScrollAlpha());
	instructions.Location.y -= 50.0f;

	instructions.Size = vec2(5.0f, 50.0f);
	instructions.BorderRadius.SetAllRadii(4.0f);
	DrawBox(Geometry, instructions);
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
}
void PictorumScrollBox::CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const {
	float offset = GetOffsetForChild(ChildIndex);

	vec2 desiredSize = Children[ChildIndex]->GetDesiredDrawSpace(CurrentRenderGeometry);

	OutputGeometry.AddLocation(0.0f, CurrentRenderGeometry.GetAllotedSpace().y);
	OutputGeometry.AddLocation(0.0f, -offset);
	OutputGeometry.AddLocation(0.0f, ScrollOffset);
	OutputGeometry.AddLocation(0.0f, -desiredSize.y);
	OutputGeometry.SetAllotedSpace(desiredSize);

	OutputGeometry.SetMinimumClipPoint(CurrentRenderGeometry.GetLocation());
	OutputGeometry.SetMaximumClipPoint(CurrentRenderGeometry.GetAllotedSpace());
}
vec2 PictorumScrollBox::GetDesiredDrawSpace(const FRenderGeometry& Geometry) const {
	vec2 size = vec2(0.0f, 0.0f);

	for (PictorumWidget* child : Children) {
		vec2 desired = child->GetDesiredDrawSpace(Geometry);
		size.x = MathLibrary::Max(desired.x, size.x);
		size.y += desired.y;
	}
	return size;
}


void PictorumScrollBox::AddScroll(const float& ScrollAmount) {
	ScrollOffset = ScrollOffset + ScrollAmount;
	PreventOverscroll();
}
void PictorumScrollBox::SetScrollPosition(const float& ScrollPosition) {
	ScrollOffset = ScrollPosition;
	PreventOverscroll();
}

void PictorumScrollBox::OnMouseMove(const vec2& MousePosition, const vec2& MouseDelta, FUserInterfaceEvent& EventIn) {
	if (bWasRightClickedIn) {
		AddScroll(MouseDelta.y);
		LastMouseDelta = MouseDelta;
	}
}
void PictorumScrollBox::OnMouseDown(const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& EventIn) {
	if (Button == EMouseButton::RIGHT) {
		bWasRightClickedIn = true;
	}
}
void PictorumScrollBox::OnMouseUp(const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& EventIn) {
	if (bWasRightClickedIn) {
		ScrollEnergy = LastMouseDelta.y  * 10.0f;
		bWasRightClickedIn = false;
	}
}
void PictorumScrollBox::OnMouseScroll(const float Delta, FUserInterfaceEvent& EventIn) {
	AddScroll(-Delta * ScrollSpeed);
}

const float PictorumScrollBox::GetOffsetForChild(const int32& ChildIndex) const {
	float offset = 0.0f;
	for (int i = 0; i < MathLibrary::Min(ChildIndex, Children.Count()); i++) {
		offset += Children[i]->GetDesiredDrawSpace(LastRenderedGeometry).y;
	}
	return offset;
}
const float PictorumScrollBox::GetScrollAlpha() const {
	return ScrollOffset / MathLibrary::Max(0.00001f, GetDesiredDrawSpace(LastRenderedGeometry).y); // Cover for the case where the desired is 0. NO DIVIDE BY ZERO HERE.
}
void PictorumScrollBox::PreventOverscroll() {
	if (ScrollOffset > GetDesiredDrawSpace(LastRenderedGeometry).y - LastRenderedGeometry.GetAllotedSpace().y) {
		ScrollOffset = GetDesiredDrawSpace(LastRenderedGeometry).y - LastRenderedGeometry.GetAllotedSpace().y;
	}
	if (ScrollOffset < 0.0f) {
		ScrollOffset = 0.0f;
	}
}