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

PictorumWidget::PictorumWidget() {
	Location = vec2(0.5f, 0.5f);
	Scale = vec2(0.1f, 0.1f);

	Rotation = 0.0f;

	DragFloat* location = new DragFloat("Location");
	location->AddEntry(&Location.x, "X %.3fpx", FColor(1.0f, 0.0f, 0.0f), 0.0f, 0.0f, 1.0f, 2.0f);
	location->AddEntry(&Location.y, "Y %.3fpx", FColor(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 1.0f, 2.0f);
	DetailsPanelWidgets.Add(location);

	DragFloat* scale = new DragFloat("Scale");
	scale->AddEntry(&Scale.x, "Width %.3fpx", FColor(1.0f, 0.0f, 0.0f), 0.0f, 0.0f, 1.0f, 2.0f);
	scale->AddEntry(&Scale.y, "Height %.3fpx", FColor(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 1.0f, 2.0f);
	DetailsPanelWidgets.Add(scale);

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
void PictorumWidget::Tick(float DeltaTime) {

}
void PictorumWidget::Draw(float DeltaTime) {
	
}
void PictorumWidget::CalculateBounds(vec2 RenderTargetResolution, vec2& MinBounds, vec2& MaxBounds) {
	MinBounds = Location - (Scale/2.0f);
	MaxBounds = Location + (Scale/2.0f);
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

mat4 PictorumWidget::CalculateModelMatrix(const vec2& ScreenDimensions) const {
	vec2 adjustedLocation = vec2((Location.x * 2.0f) - ScreenDimensions.x, ScreenDimensions.y - (Location.y * 2.0f));

	mat4 posMatrix = glm::translate(vec3(adjustedLocation.x / ScreenDimensions.x, adjustedLocation.y / ScreenDimensions.y, 0.0f));
	mat4 scaleMatrix = glm::scale(vec3(Scale.x / ScreenDimensions.x, Scale.y / ScreenDimensions.y, 0.0f));
	mat4 rotXMatrix = glm::rotate(0.0f, vec3(1, 0, 0));
	mat4 rotYMatrix = glm::rotate(0.0f, vec3(0, 1, 0));
	mat4 rotZMatrix = glm::rotate(Rotation, vec3(0, 0, 1));

	mat4 combinedRotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;

	return posMatrix * scaleMatrix * combinedRotMatrix;
}
void PictorumWidget::DrawContents(float DeltaTime) {
	Draw(DeltaTime);
	for (PictorumWidget* widget : Children) {
		widget->DrawContents(DeltaTime);
	}
}

void PictorumWidget::OnMouseEnter(vec2 MousePosition, FUserInterfaceEvent& Event) {}
void PictorumWidget::OnMouseExit(vec2 MousePosition, FUserInterfaceEvent& Event) {}
void PictorumWidget::OnMouseMove(vec2 MousePosition, vec2 MouseDelta, FUserInterfaceEvent& Event) {}
void PictorumWidget::OnMouseDown(vec2 MousePosition, EMouseButton Button, FUserInterfaceEvent& Event) {}
void PictorumWidget::OnMouseUp(vec2 MousePosition, EMouseButton Button, FUserInterfaceEvent& Event) {}

TString PictorumWidget::GetDetailsPanelName() {
	return "UI Widget";
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