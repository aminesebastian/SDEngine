#include "HorizontalBoxWidget.h"

HorizontalBoxWidget::HorizontalBoxWidget(const TString& Name) : PictorumWidget(Name) {
	SetVisibility(EPictorumVisibilityState::SELF_HIT_TEST_INVISIBLE);
	///< An enum constant representing the last X coordinate position option
	LastXPosition = 0.0f;
}
const bool HorizontalBoxWidget::CanAddChild() const
{
	return true;
}
void HorizontalBoxWidget::CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const {
	if (ChildIndex == 0) {
		const_cast<HorizontalBoxWidget*>(this)->LastXPosition = 0.0f;
	}

	HorizontalBoxSlot* slot = GetChildSlotAtIndex<HorizontalBoxSlot>(ChildIndex);
	OutputGeometry.SetLocation(vec2(LastXPosition, OutputGeometry.GetLocation().y));
	vec2 desiredSpace   = Children[ChildIndex]->GetDesiredDrawSpace(OutputGeometry);

	if (slot->GetFillRule() == EFillRule::AUTOMATIC) {
		OutputGeometry.SetAllotedSpace(desiredSpace);
	} else {
		float ratio   = GetFillRatioForChild(ChildIndex);
		vec2 space    = CurrentRenderGeometry.GetAllotedSpace();
		space.x      -= GetNonFillSpaceRequirements(CurrentRenderGeometry);
		space.x      /= ratio;
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

	// Breaking const to capture temporary variable.
	const_cast<HorizontalBoxWidget*>(this)->LastXPosition += OutputGeometry.GetAllotedSpace().x;
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