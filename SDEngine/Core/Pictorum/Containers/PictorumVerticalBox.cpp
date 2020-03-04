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
	OutputGeometry.AddLocation(0.0f, OutputGeometry.GetAllotedSpace().y-yOffset);

	vec2 desiredSpace = Children[ChildIndex]->GetDesiredDrawSpace(OutputGeometry);
	desiredSpace.x = MathLibrary::Min(desiredSpace.x, CurrentRenderGeometry.GetAllotedSpace().x);
	desiredSpace.y = MathLibrary::Min(desiredSpace.y, CurrentRenderGeometry.GetAllotedSpace().y);
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

	vec2 location = OutputGeometry.GetLocation();
	switch (slot->GetHorizontalAlignment()) {
		case EHorizontalAlignment::LEFT:
			break;
		case EHorizontalAlignment::CENTER:
			location.x = OutputGeometry.GetLocation().x - desiredSpace.x / 2.0f;
			break;
		case EHorizontalAlignment::RIGHT:
			break;
		case EHorizontalAlignment::STRETCH:
			OutputGeometry.SetAllotedSpace(Vector2D(CurrentRenderGeometry.GetAllotedSpace().x, OutputGeometry.GetAllotedSpace().y));
			break;
	}

	OutputGeometry.SetLocation(location);

	// Apply the padding.
	OutputGeometry.AddLocation(slot->GetPadding().GetLeft(), slot->GetPadding().GetBottom());
	OutputGeometry.AddAllotedSpace(slot->GetPadding().GetRight(), slot->GetPadding().GetTop());
}
vec2 PictorumVerticalBox::GetDesiredDrawSpace(const FRenderGeometry& Geometry) const {
	vec2 desiredSize = ZERO_VECTOR2D;
	for (uint8 i = 0; i < Children.Count(); i++) {
		PictorumWidget* child = Children[i];
		vec2 childSpace = child->GetDesiredDrawSpace(Geometry);
		desiredSize.x = MathLibrary::Max(desiredSize.x, childSpace.x);
		desiredSize.y += CalculateChildHeight(Geometry, i);
	}
	return desiredSize;
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