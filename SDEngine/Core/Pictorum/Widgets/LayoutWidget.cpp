#include "LayoutWidget.h"
#include "UserInterface/Widgets/DragFloat.h"

LayoutWidget::LayoutWidget(TString Name) : PictorumWidget(Name) {
	Anchors.Left = 0.0f;
	Anchors.Right = 1.0f;
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

void LayoutWidget::Tick(float DeltaTime, FRenderGeometry Geometry) {
	UpdateScale(Geometry);
}
void LayoutWidget::UpdateScale(FRenderGeometry Geometry) {
	if (Anchors.bIsRelative) {
		Scale.x = Geometry.RenderResolution.x * (Anchors.Right - Anchors.Left);
		Scale.y = Geometry.RenderResolution.x * (Anchors.Top - Anchors.Bottom);

		Location.x = (Geometry.RenderResolution.x * (Anchors.Right - Anchors.Left))/2.0f;
		Location.y = (Geometry.RenderResolution.x * (Anchors.Top - Anchors.Bottom))/2.0f;
	} else {
		Scale.x = (Anchors.Right - Anchors.Left);
		Scale.y = (Anchors.Top - Anchors.Bottom);

		Location.x = (Anchors.Right - Anchors.Left)/2.0f;
		Location.y = (Anchors.Top - Anchors.Bottom)/2.0f;
	}
}
bool LayoutWidget::PopulateDetailsPanel() {
	PictorumWidget::PopulateDetailsPanel();

	ImGui::Text("Anchors Relative");
	ImGui::Checkbox("AnchorsRelative", &Anchors.bIsRelative);

	return true;
}
