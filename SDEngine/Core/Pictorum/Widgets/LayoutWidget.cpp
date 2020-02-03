#include "LayoutWidget.h"
#include "UserInterface/Widgets/DragFloat.h"

LayoutWidget::LayoutWidget(TString Name) : PictorumWidget(Name) {
	Anchors.Left = 0.0f;
	Anchors.Right = 0.25f;
	Anchors.Top = 1.0f;
	Anchors.Bottom = 0.0f;
	Anchors.bIsRelative = true;

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
void LayoutWidget::CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) {
	PictorumWidget* child = Children[ChildIndex];
	vec2 screenResolution = CurrentRenderGeometry.GetRenderResolution();
	vec2 offset           = child->GetPivotOffset() + 0.5f;
	vec2 location;
	vec2 space;

	// Calculate anchors.
	if (Anchors.bIsRelative) {
		space.x = CurrentRenderGeometry.GetRenderResolution().x * (Anchors.Right - Anchors.Left);
		space.y = CurrentRenderGeometry.GetRenderResolution().y * (Anchors.Top - Anchors.Bottom);

		location.x = (CurrentRenderGeometry.GetRenderResolution().x *  Anchors.Left);
		location.y = (CurrentRenderGeometry.GetRenderResolution().y *  Anchors.Bottom);
	} else {
		space.x = (Anchors.Right - Anchors.Left);
		space.y = (Anchors.Top - Anchors.Bottom);

		location.x = Anchors.Left;
		location.y = Anchors.Bottom;
	}

	// Apply offsets.
	location.x += space.x * offset.x;
	location.y += space.y * offset.y;

	// Update the location.
	OutputGeometry.SetLocation(location);

	// Set the space depending on the fill amount.
	if (child->GetFillState() == EFillState::AUTOMATIC) {
		OutputGeometry.SetAllocatedSpace(child->GetDesiredDrawSpace(OutputGeometry));
	} else {
		OutputGeometry.SetAllocatedSpace(space);
	}
}
bool LayoutWidget::PopulateDetailsPanel() {
	PictorumWidget::PopulateDetailsPanel();

	ImGui::Text("Anchors Relative");
	ImGui::Checkbox("AnchorsRelative", &Anchors.bIsRelative);

	return true;
}
