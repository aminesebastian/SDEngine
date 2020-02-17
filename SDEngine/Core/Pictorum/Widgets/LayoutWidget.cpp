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

	const FAnchors& anchors = slot->GetAnchors();
	const FOffsets& offsets = slot->GetOffsets();
	const FPivotOffset& pivotOffset = slot->GetPivotOffset();

	if (anchors.GetLeft() == anchors.GetRight()) {

	} else if (anchors.GetTop() == anchors.GetBottom()) {

	} else {

	}


	OutputGeometry.SetMaximumClipPoint(OutputGeometry.GetLocation() + OutputGeometry.GetAllotedSpace());
	OutputGeometry.SetMinimumClipPoint(OutputGeometry.GetLocation());
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
