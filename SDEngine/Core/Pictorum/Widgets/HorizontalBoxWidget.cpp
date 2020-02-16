#include "HorizontalBoxWidget.h"
#include "Core/Utilities/Math/MathLibrary.h"

HorizontalBoxWidget::HorizontalBoxWidget(const TString& Name) : PictorumWidget(Name) {
	SetVisibility(EPictorumVisibilityState::SELF_HIT_TEST_INVISIBLE);
}
const bool HorizontalBoxWidget::CanAddChild() const {
	return true;
}
void HorizontalBoxWidget::CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const {
	float xOffset           = CalculateXOffsetForChild(CurrentRenderGeometry, ChildIndex - 1);
	vec2 desiredSpace       = Children[ChildIndex]->GetDesiredDrawSpace(OutputGeometry);
	desiredSpace.x          = MathLibrary::Min(desiredSpace.x, CurrentRenderGeometry.GetAllotedSpace().x);
	desiredSpace.y          = MathLibrary::Min(desiredSpace.y, CurrentRenderGeometry.GetAllotedSpace().y);

	OutputGeometry.AddLocation(xOffset, 0.0f);

	HorizontalBoxSlot* slot = GetChildSlotAtIndex<HorizontalBoxSlot>(ChildIndex);
	if (slot->GetFillRule() == EFillRule::AUTOMATIC) {
		OutputGeometry.SetAllotedSpace(desiredSpace);
	} else {
		float ratio = GetFillRatioForChild(ChildIndex);
		vec2 space = CurrentRenderGeometry.GetAllotedSpace();
		space.x -= GetNonFillSpaceRequirements(CurrentRenderGeometry);
		space.x /= ratio;
		OutputGeometry.SetAllotedSpace(space);
	}

	vec2 location = OutputGeometry.GetLocation();
	switch (slot->GetVerticalAlignment()) {
		case EVerticalAlignment::TOP:
			location.y = (CurrentRenderGeometry.GetLocation().y + CurrentRenderGeometry.GetAllotedSpace().y) - desiredSpace.y;
			break;
		case EVerticalAlignment::CENTER:
			location.y = OutputGeometry.GetLocation().y - desiredSpace.y / 2.0f;
			break;
		case EVerticalAlignment::BOTTOM:
			break;
		case EVerticalAlignment::STRETCH:
			break;
	}

	OutputGeometry.SetLocation(location);

	// Apply the padding.
	slot->GetPadding().ApplyToGeometry(OutputGeometry, OutputGeometry);
}
vec2 HorizontalBoxWidget::GetDesiredDrawSpace(const FRenderGeometry& Geometry) const {
	vec2 desiredSize = ZERO_VECTOR2D;
	for (uint8 i = 0; i < Children.Count(); i++) {
		PictorumWidget* child = Children[i];
		vec2 childSpace = child->GetDesiredDrawSpace(Geometry);
		desiredSize.y = MathLibrary::Max(desiredSize.y, childSpace.y);
		desiredSize.x += CalculateChildWidth(Geometry, i);
	}
	return desiredSize;
}

float HorizontalBoxWidget::CalculateXOffsetForChild(const FRenderGeometry& CurrentRenderGeometry, const int32& ChildIndex) const {
	float offset = 0.0f;

	for (int32 i = 0; i <= ChildIndex; i++) {
		offset += CalculateChildWidth(CurrentRenderGeometry, i);
	}

	return offset;
}
float HorizontalBoxWidget::CalculateChildWidth(const FRenderGeometry& CurrentRenderGeometry,const int32 ChildIndex) const {
	HorizontalBoxSlot* slot = GetChildSlotAtIndex<HorizontalBoxSlot>(ChildIndex);
	float desiredX          = Children[ChildIndex]->GetDesiredDrawSpace(CurrentRenderGeometry).x;
	desiredX                = MathLibrary::Min(desiredX, CurrentRenderGeometry.GetAllotedSpace().x);

	if (slot->GetFillRule() == EFillRule::AUTOMATIC) {
		return desiredX;
	} else {
		float ratio = GetFillRatioForChild(ChildIndex);
		float filledXSpace = CurrentRenderGeometry.GetAllotedSpace().x;
		filledXSpace -= GetNonFillSpaceRequirements(CurrentRenderGeometry);
		filledXSpace /= ratio;
		return filledXSpace;
	}
	return 0.0f;
}

HorizontalBoxSlot* HorizontalBoxWidget::CreateSlotForWidget(PictorumWidget* WidgetForSlot) const {
	return new HorizontalBoxSlot();
}
HorizontalBoxSlot* HorizontalBoxWidget::AddChild(PictorumWidget* Widget) {
	return Cast<HorizontalBoxSlot>(AddChildInternal(Widget));
}

float HorizontalBoxWidget::GetFillRatioForChild(int32 ChildIndex) const {
	HorizontalBoxSlot* childSlot = GetChildSlotAtIndex<HorizontalBoxSlot>(ChildIndex);
	if (childSlot->GetFillRule() == EFillRule::AUTOMATIC) {
		return 0.0f;
	}

	float sum = 0.0f;

	for (const PictorumWidget* child : Children) {
		HorizontalBoxSlot* slot = child->GetParentSlot<HorizontalBoxSlot>();
		if (slot->GetFillRule() == EFillRule::FILL) {
			sum += slot->GetFillRatio();
		}
	}

	return childSlot->GetFillRatio() / sum;
}
float HorizontalBoxWidget::GetNonFillSpaceRequirements(const FRenderGeometry& CurrentRenderGeometry) const {
	float widthRequirement = 0.0f;

	for (const PictorumWidget* child : Children) {
		HorizontalBoxSlot* slot = child->GetParentSlot<HorizontalBoxSlot>();
		if (slot->GetFillRule() == EFillRule::AUTOMATIC) {
			widthRequirement += child->GetDesiredDrawSpace(CurrentRenderGeometry).x;
		}
	}

	return widthRequirement;
}