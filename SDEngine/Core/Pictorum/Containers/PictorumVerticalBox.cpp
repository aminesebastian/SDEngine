#include "PictorumVerticalBox.h"
#include "Core/Utilities/Math/MathLibrary.h"

PictorumVerticalBox::PictorumVerticalBox(const TString& Name) : PictorumWidget(Name) {
	SetVisibility(EPictorumVisibilityState::SELF_HIT_TEST_INVISIBLE);
}
const bool PictorumVerticalBox::CanAddChild() const {
	return true;
}
void PictorumVerticalBox::CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const {
	float yOffset = CalculateYOffsetForChild(CurrentRenderGeometry, ChildIndex - 1);
	OutputGeometry.AddLocation(0.0f, OutputGeometry.GetAllotedSpace().y-yOffset-Padding.GetTop());

	// Capture the desired space of the child and ensure it is kept smaller than the allocated space.
	vec2 desiredSpace = Children[ChildIndex]->GetDesiredDrawSpace(OutputGeometry);
	desiredSpace.x = MathLibrary::Min(desiredSpace.x, CurrentRenderGeometry.GetAllotedSpace().x);
	desiredSpace.y = MathLibrary::Min(desiredSpace.y, CurrentRenderGeometry.GetAllotedSpace().y);

	// Move the location to BOTTOM of where the child will render.
	OutputGeometry.AddLocation(0.0f, -desiredSpace.y);

	VerticalBoxSlot* slot = GetChildSlotAtIndex<VerticalBoxSlot>(ChildIndex);
	if (slot->GetFillRule() == EFillRule::AUTOMATIC) {
		OutputGeometry.SetAllotedSpace(desiredSpace);
	} else {
		float ratio = GetFillRatioForChild(ChildIndex);
		vec2 space = CurrentRenderGeometry.GetAllotedSpace();
		space.y -= GetNonFillSpaceRequirements(CurrentRenderGeometry);
		space.y /= ratio;
		OutputGeometry.SetAllotedSpace(space);
	}

	Vector2D location = OutputGeometry.GetLocation();
	Vector2D space = OutputGeometry.GetAllotedSpace();
	switch (slot->GetHorizontalAlignment()) {
		case EHorizontalAlignment::LEFT:
			break;
		case EHorizontalAlignment::CENTER:
			location.x = OutputGeometry.GetLocation().x - space.x / 2.0f;
			break;
		case EHorizontalAlignment::RIGHT:
			break;
		case EHorizontalAlignment::STRETCH:
			OutputGeometry.SetAllotedSpace(Vector2D(CurrentRenderGeometry.GetAllotedSpace().x, space.y));
			break;
	}

	OutputGeometry.SetLocation(location);

	// Apply the padding.
	OutputGeometry.AddLocation(slot->GetPadding().GetLeft(), slot->GetPadding().GetBottom());
	OutputGeometry.AddAllotedSpace(slot->GetPadding().GetRight(), slot->GetPadding().GetTop());

	OutputGeometry.AddLocation(Padding.GetLeft(), Padding.GetBottom());
	OutputGeometry.AddAllotedSpace(-Padding.GetLeft() - Padding.GetRight(), 0.0f);
}
vec2 PictorumVerticalBox::GetDesiredDrawSpace(const FRenderGeometry& Geometry) const {
	vec2 desiredSize = ZERO_VECTOR2D;
	for (uint8 i = 0; i < Children.Count(); i++) {
		PictorumWidget* child = Children[i];
		vec2 childSpace = child->GetDesiredDrawSpace(Geometry);
		desiredSize.x = MathLibrary::Max(desiredSize.x, childSpace.x);
		desiredSize.y += CalculateChildHeight(Geometry, i);
	}
	return desiredSize + Vector2D(Padding.GetLeft() + Padding.GetRight(), Padding.GetTop() + Padding.GetBottom());
}

float PictorumVerticalBox::CalculateYOffsetForChild(const FRenderGeometry& CurrentRenderGeometry, const int32& ChildIndex) const {
	float offset = 0.0f;

	for (int32 i = 0; i <= ChildIndex; i++) {
		offset += CalculateChildHeight(CurrentRenderGeometry, i);
	}

	return offset;
}
float PictorumVerticalBox::CalculateChildHeight(const FRenderGeometry& CurrentRenderGeometry, const int32 ChildIndex) const {
	VerticalBoxSlot* slot = GetChildSlotAtIndex<VerticalBoxSlot>(ChildIndex);
	float desiredY        = Children[ChildIndex]->GetDesiredDrawSpace(CurrentRenderGeometry).y;
	desiredY              = MathLibrary::Min(desiredY, CurrentRenderGeometry.GetAllotedSpace().y);

	if (slot->GetFillRule() == EFillRule::AUTOMATIC) {
		return desiredY;
	} else {
		float ratio = GetFillRatioForChild(ChildIndex);
		float filledYSpace = CurrentRenderGeometry.GetAllotedSpace().y;
		filledYSpace -= GetNonFillSpaceRequirements(CurrentRenderGeometry);
		filledYSpace /= ratio;
		return filledYSpace;
	}
	return 0.0f;
}

VerticalBoxSlot* PictorumVerticalBox::CreateSlotForWidget(PictorumWidget* WidgetForSlot) const {
	return new VerticalBoxSlot();
}
VerticalBoxSlot* PictorumVerticalBox::AddChild(PictorumWidget* Widget) {
	return Cast<VerticalBoxSlot>(AddChildInternal(Widget));
}

const FPadding& PictorumVerticalBox::GetPadding() const {
	return Padding;
}
void PictorumVerticalBox::SetPadding(const float& AllPadding) {
	Padding.SetTop(AllPadding);
	Padding.SetBottom(AllPadding);
	Padding.SetRight(AllPadding);
	Padding.SetLeft(AllPadding);
}
void PictorumVerticalBox::SetPadding(const float& TopBottomPadding, const float& RightLeftPadding) {
	Padding.SetTop(TopBottomPadding);
	Padding.SetBottom(TopBottomPadding);
	Padding.SetRight(RightLeftPadding);
	Padding.SetLeft(RightLeftPadding);
}
void PictorumVerticalBox::SetPadding(const float& Top, const float& Right, const float& Bottom, const float& Left) {
	Padding.SetTop(Top);
	Padding.SetBottom(Bottom);
	Padding.SetRight(Right);
	Padding.SetLeft(Left);
}

float PictorumVerticalBox::GetFillRatioForChild(int32 ChildIndex) const {
	VerticalBoxSlot* childSlot = GetChildSlotAtIndex<VerticalBoxSlot>(ChildIndex);
	if (childSlot->GetFillRule() == EFillRule::AUTOMATIC) {
		return 0.0f;
	}

	float sum = 0.0f;

	for (const PictorumWidget* child : Children) {
		VerticalBoxSlot* slot = child->GetParentSlot<VerticalBoxSlot>();
		if (slot->GetFillRule() == EFillRule::FILL) {
			sum += slot->GetFillRatio();
		}
	}

	return childSlot->GetFillRatio() / sum;
}
float PictorumVerticalBox::GetNonFillSpaceRequirements(const FRenderGeometry& CurrentRenderGeometry) const {
	float heightRequirement = 0.0f;

	for (const PictorumWidget* child : Children) {
		VerticalBoxSlot* slot = child->GetParentSlot<VerticalBoxSlot>();
		if (slot->GetFillRule() == EFillRule::AUTOMATIC) {
			heightRequirement += child->GetDesiredDrawSpace(CurrentRenderGeometry).y;
		}
	}

	return heightRequirement;
}