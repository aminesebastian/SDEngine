#include "LayoutWidget.h"
#include "Core/Utilities/EngineFunctionLibrary.h"
#include "Core/Utilities/Logger.h"
#include "Core/Utilities/Math/MathLibrary.h"
#include "UserInterface/Widgets/DragFloat.h"

LayoutWidget::LayoutWidget(const TString& Name) : PictorumWidget(Name) {
	SetVisibility(EPictorumVisibilityState::SELF_HIT_TEST_INVISIBLE);
}
LayoutWidget::~LayoutWidget() {

}
const bool LayoutWidget::CanAddChild() const {
	return true;
}
void LayoutWidget::CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const {
	LayoutWidgetSlot* slot = GetChildSlotAtIndex<LayoutWidgetSlot>(ChildIndex);
	if (!slot) {
		SD_ENGINE_ERROR("Encountered an null slot for widget: {0} in widget: {1} that should have one!", Children[ChildIndex]->GetName(), GetName());
		return;
	}
	Anchors.ApplyToGeometry(CurrentRenderGeometry, OutputGeometry);
	slot->GetOffsets().ApplyToGeometry(OutputGeometry, OutputGeometry);

	vec2& allotedSpace = OutputGeometry.GetAllotedSpace();
	const vec2& desiredSpace = Children[ChildIndex]->GetDesiredDrawSpace(OutputGeometry);

	if (slot->GetHorizontalFillRule() == EFillRule::AUTOMATIC) {
		allotedSpace.x = MathLibrary::Min(allotedSpace.x, desiredSpace.x);
	}
	if (slot->GetVerticalFillRule() == EFillRule::AUTOMATIC) {
		allotedSpace.y = MathLibrary::Min(allotedSpace.x, desiredSpace.y);
	}
	OutputGeometry.SetAllotedSpace(allotedSpace);
}
LayoutWidget* LayoutWidget::SetAnchor(const EPictorumSide& Side, const float& AnchorPosition) {
	Anchors.SetSide(Side, AnchorPosition);
	return this;
}
LayoutWidgetSlot* LayoutWidget::AddChild(PictorumWidget* Widget) {
	return Cast<LayoutWidgetSlot>(AddChildInternal(Widget));
}
LayoutWidgetSlot* LayoutWidget::CreateSlotForWidget(PictorumWidget* WidgetForSlot) const {
	return new LayoutWidgetSlot();
}
bool LayoutWidget::PopulateDetailsPanel() {
	PictorumWidget::PopulateDetailsPanel();

	return true;
}
