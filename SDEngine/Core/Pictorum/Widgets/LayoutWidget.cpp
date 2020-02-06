#include "LayoutWidget.h"
#include "Utilities/EngineFunctionLibrary.h"
#include "Utilities/Logger.h"
#include "UserInterface/Widgets/DragFloat.h"

LayoutWidget::LayoutWidget(const TString& Name) : PictorumWidget(Name) {
	Anchors.Left = 0.0f;
	Anchors.Right = 0.25f;
	Anchors.Top = 1.0f;
	Anchors.Bottom = 0.0f;

	DragFloat* anchorsControl = new DragFloat("Anchors");
	anchorsControl->AddEntry(&Anchors.Top, "Top %.3fpx", FColor(1.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.1f, 2.0f);
	anchorsControl->AddEntry(&Anchors.Right, "Right %.3fpx", FColor(1.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.1f, 2.0f);
	anchorsControl->AddEntry(&Anchors.Bottom, "Bottom %.3fpx", FColor(1.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.1f, 2.0f);
	anchorsControl->AddEntry(&Anchors.Left, "Left %.3fpx", FColor(1.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.1f, 2.0f);
	DetailsPanelWidgets.Add(anchorsControl);

	SetVisibility(EPictorumVisibilityState::SELF_HIT_TEST_INVISIBLE);
}
LayoutWidget::~LayoutWidget() {

}
bool LayoutWidget::CanAddChild() const {
	return true;
}
void LayoutWidget::CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const {
	// Apply the anchors and padding.
	Anchors.ApplyAnchorsToGeometry(CurrentRenderGeometry, OutputGeometry);
	Padding.ApplyPaddingToGeometry(OutputGeometry, OutputGeometry);

	PictorumWidget* child  = Children[ChildIndex];
	LayoutWidgetSlot* slot = Cast<LayoutWidgetSlot>(child->GetParentSlot());
	if (!slot) {
		SD_ENGINE_ERROR("Encountered an null slot for widget: {0} in widget: {1} that should have one!", child->GetName(), GetName());
		return;
	}
	slot->GetOffsets().ApplyAnchorsToGeometry(OutputGeometry, OutputGeometry);
	slot->GetMargins().ApplyMarginsToGeometry(OutputGeometry, OutputGeometry);
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
