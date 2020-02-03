#include "PictorumWidget.h"
#include "Engine/EngineStatics.h"
#include "Engine/Engine.h"
#include "Engine/RenderViewport.h"
#include "Rendering/DefferedCompositor.h"
#include "Rendering/Shader.h"
#include "UserInterface/UserInterface.h"
#include "UserInterface/UserInterfaceUtilities.h"
#include "UserInterface/Widgets/DragFloat.h"
#include "Utilities/Logger.h"

PictorumWidget::PictorumWidget(TString Name) : EngineObject(Name) {
	Rotation            = 0.0f; // 0 Degrees
	Parent              = nullptr;
	Visibility          = EPictorumVisibilityState::VISIBLE;
	HorizontalAlignment = EHorizontalAlignment::LEFT;
	VerticalAlignment   = EVerticalAlignment::LEFT;
	FillState           = EFillState::FILL_ALL_SPACE;
	PivotOffset		    = vec2(0.0f, 0.0f);

	std::function<float(float)> toDegrees = ([](float radians) { return glm::degrees(radians); });
	std::function<float(float)> toRadians = ([](float degrees) { return glm::radians(degrees); });

	DragFloat* rotation = new DragFloat("Angle");
	FDragFloatEntry* z = rotation->AddEntry(&Rotation, "%.3f", FColor(1.0f, 0.0f, 0.0f), 0.0f, 0.0f, 1.0f, 2.0f);
	z->OnFormatForViewFunction = toDegrees;
	z->OnFormatFromView = toRadians;

	DetailsPanelWidgets.Add(rotation);
}
PictorumWidget::~PictorumWidget() {

}
void PictorumWidget::OnCreated(FRenderGeometry Geometry) {

}
void PictorumWidget::OnDestroyed(FRenderGeometry Geometry) {

}
void PictorumWidget::Tick(float DeltaTime, const FRenderGeometry& Geometry) {

}
void PictorumWidget::Draw(float DeltaTime, const FRenderGeometry& Geometry) {

}
vec2 PictorumWidget::GetDesiredDrawSpace(const FRenderGeometry& Geometry) {
	return vec2(100.0f, 100.0f);
}
void PictorumWidget::CalculateBounds(vec2 RenderTargetResolution, vec2& MinBounds, vec2& MaxBounds) {
	vec2 lastLocation = LastRenderedGeometry.GetLocation(EPictorumLocationBasis::ABSOLUTE);
	vec2 lastScale = LastRenderedGeometry.GetAllocatedSpace(EPictorumScaleBasis::ABSOLUTE);
	MinBounds = lastLocation - (lastScale / 2.0f);
	MaxBounds = lastLocation + (lastScale / 2.0f);
}
void PictorumWidget::CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) {
	OutputGeometry = CurrentRenderGeometry;
}

bool PictorumWidget::AddChild(PictorumWidget* Widget) {
	if (Children.AddUnique(Widget)) {
		Widget->OnAddedToParent(this);
		return true;
	}
	return false;
}
bool PictorumWidget::RemoveChild(PictorumWidget* Widget) {
	if (Children.Remove(Widget)) {
		Widget->OnRemovedFromParent(this);
		return true;
	}
	return false;
}

void PictorumWidget::OnAddedToParent(PictorumWidget* ParentIn) {
	Parent = ParentIn;
}
void PictorumWidget::OnRemovedFromParent(PictorumWidget* ParentIn) {
	Parent = nullptr;
}

mat4 PictorumWidget::CalculateModelMatrix(const FRenderGeometry& Geometry) const {
	vec2 relativeLocation = Geometry.GetLocation(EPictorumLocationBasis::NDC);
	vec2 relativeScale    = Geometry.GetAllocatedSpace(EPictorumScaleBasis::RELATIVE);
	float screenRotation  = GetRenderRotation();

	mat4 posMatrix   = glm::translate(vec3(relativeLocation, 0.0f));
	mat4 scaleMatrix = glm::scale(vec3(relativeScale, 0.0f));
	mat4 rotXMatrix  = glm::rotate(0.0f, vec3(1, 0, 0));
	mat4 rotYMatrix  = glm::rotate(0.0f, vec3(0, 1, 0));
	mat4 rotZMatrix  = glm::rotate(screenRotation, vec3(0, 0, 1));

	mat4 combinedRotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;

	return posMatrix * scaleMatrix * combinedRotMatrix;
}
void PictorumWidget::DrawContents(float DeltaTime, FRenderGeometry Geometry) {
	LastRenderedGeometry = Geometry;

	Draw(DeltaTime, Geometry);

	for (int i = 0; i < Children.Count(); i++) {
		PictorumWidget* widget = Children[i];
		FRenderGeometry childGeometry;
		childGeometry.SetRenderResolution(Geometry.GetRenderResolution());
		CalculateChildRenderGeometry(Geometry, childGeometry, i);
		widget->DrawContents(DeltaTime, childGeometry);
	}
}

void PictorumWidget::OnMouseEnter(vec2 MousePosition, FUserInterfaceEvent& Event) {}
void PictorumWidget::OnMouseExit(vec2 MousePosition, FUserInterfaceEvent& Event) {}
void PictorumWidget::OnMouseMove(vec2 MousePosition, vec2 MouseDelta, FUserInterfaceEvent& Event) {}
void PictorumWidget::OnMouseDown(vec2 MousePosition, EMouseButton Button, FUserInterfaceEvent& Event) {}
void PictorumWidget::OnMouseUp(vec2 MousePosition, EMouseButton Button, FUserInterfaceEvent& Event) {}

void PictorumWidget::GetAllChildren(SArray<PictorumWidget*>& ChildrenOut) const {
	ChildrenOut.AddAll(Children);
	for (PictorumWidget* widget : Children) {
		widget->GetAllChildren(ChildrenOut);
	}
}
PictorumWidget* PictorumWidget::GetParent() const {
	return Parent;
}

void PictorumWidget::SetHorizontalAlignment(EHorizontalAlignment Alignment) {
	HorizontalAlignment = Alignment;
}
EHorizontalAlignment PictorumWidget::GetHorizontalAlignment() const {
	return HorizontalAlignment;
}
void PictorumWidget::SetVerticalAlignment(EVerticalAlignment Alignment) {
	VerticalAlignment = Alignment;
}
EVerticalAlignment PictorumWidget::GetVerticalAlignment() const {
	return VerticalAlignment;
}
void PictorumWidget::SetFillState(EFillState State) {
	FillState = State;
}
EFillState PictorumWidget::GetFillState() const {
	return FillState;
}

void PictorumWidget::SetMargins(FMargins NewMargins) {
	Margins = NewMargins;
}
void PictorumWidget::SetMargins(float Top, float Right, float Left, float Bottom) {
	Margins.Top = Top;
	Margins.Right = Right;
	Margins.Bottom = Bottom;
	Margins.Left = Left;
}
FMargins PictorumWidget::GetMargins() const {
	return Margins;
}
void PictorumWidget::SetPadding(FPadding NewPadding) {
	Padding = NewPadding;
}
void PictorumWidget::SetPadding(float Top, float Right, float Left, float Bottom) {
	Padding.Top = Top;
	Padding.Right = Right;
	Padding.Bottom = Bottom;
	Padding.Left = Left;
}
FPadding PictorumWidget::GetPadding() const {
	return Padding;
}
void PictorumWidget::SetVisibility(EPictorumVisibilityState NewVisibility) {
	Visibility = NewVisibility;
}
EPictorumVisibilityState PictorumWidget::GetVisibility() const {
	return Visibility;
}
vec2 PictorumWidget::GetPivotOffset() const {
	return PivotOffset;
}

float PictorumWidget::GetRenderRotation() const {
	return GetRotation() + GetParentRotation();
}
float PictorumWidget::GetRotation() const {
	return Rotation;
}
float PictorumWidget::GetParentRotation() const {
	if (!Parent) {
		return 0.0f;
	}
	return Parent->GetRotation();
}

TString PictorumWidget::GetDetailsPanelName() {
	return GetName();
}
bool PictorumWidget::PopulateDetailsPanel() {
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Transform", flags)) {
		for (DragFloat* widget : DetailsPanelWidgets) {
			widget->Draw();
		}
	}

	return true;
}