#include "LayoutWidget.h"
#include "Core/Utilities/EngineFunctionLibrary.h"
#include "Core/Utilities/Logger.h"
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
	// Apply the anchors and padding.
	Anchors.ApplyToGeometry(CurrentRenderGeometry, OutputGeometry);
	Padding.ApplyToGeometry(OutputGeometry, OutputGeometry);

	LayoutWidgetSlot* slot = GetChildSlotAtIndex<LayoutWidgetSlot>(ChildIndex);
	if (!slot) {
		SD_ENGINE_ERROR("Encountered an null slot for widget: {0} in widget: {1} that should have one!", Children[ChildIndex]->GetName(), GetName());
		return;
	}
	slot->GetOffsets().ApplyToGeometry(OutputGeometry, OutputGeometry);
	slot->GetMargins().ApplyToGeometry(OutputGeometry, OutputGeometry);
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
