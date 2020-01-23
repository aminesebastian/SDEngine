#include "Actor.h"
#include "Entities/Camera.h"
#include "Entities/Component.h"
#include "Entities/Entity.h"
#include "UserInterface/UserInterface.h"
#include "UserInterface/UserInterfaceUtilities.h"
#include "UserInterface/Widgets/DragFloat.h"

Actor::Actor(const TString& Name) : Entity(Name) {
	RootComponent = nullptr;
	DragFloat* location = new DragFloat("Location");
	location->AddEntry(&CurrentTransform.GetLocation().x, "X %.3f", FColor(1.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.01f, 2.0f);
	location->AddEntry(&CurrentTransform.GetLocation().y, "Y %.3f", FColor(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 0.01f, 2.0f);
	location->AddEntry(&CurrentTransform.GetLocation().z, "Z %.3f", FColor(0.0f, 0.0f, 1.0f), 0.0f, 0.0f, 0.01f, 2.0f);
	DetailsPanelWidgets.push_back(location);

	std::function<float(float)> toDegrees = ([](float radians) { return glm::degrees(radians); });
	std::function<float(float)> toRadians = ([](float degrees) { return glm::radians(degrees); });

	DragFloat* rotation = new DragFloat("Rotation");
	FDragFloatEntry* x = rotation->AddEntry(&CurrentTransform.GetRotation().x, "P %.3f", FColor(1.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.1f, 2.0f);
	x->OnFormatForViewFunction = toDegrees;
	x->OnFormatFromView = toRadians;

	FDragFloatEntry* y = rotation->AddEntry(&CurrentTransform.GetRotation().y, "Y %.3f", FColor(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 0.1f, 2.0f);
	y->OnFormatForViewFunction = toDegrees;
	y->OnFormatFromView = toRadians;

	FDragFloatEntry* z = rotation->AddEntry(&CurrentTransform.GetRotation().z, "R %.3f", FColor(0.0f, 0.0f, 1.0f), 0.0f, 0.0f, 0.1f, 2.0f);
	z->OnFormatForViewFunction = toDegrees;
	z->OnFormatFromView = toRadians;

	DetailsPanelWidgets.push_back(rotation);

	DragFloat* scale = new DragFloat("Scale");
	scale->AddEntry(&CurrentTransform.GetScale().x, "X %.3f", FColor(1.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.01f, 2.0f);
	scale->AddEntry(&CurrentTransform.GetScale().y, "Y %.3f", FColor(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 0.01f, 2.0f);
	scale->AddEntry(&CurrentTransform.GetScale().z, "Z %.3f", FColor(0.0f, 0.0f, 1.0f), 0.0f, 0.0f, 0.01f, 2.0f);
	DetailsPanelWidgets.push_back(scale);

}
Actor::~Actor() {}

void Actor::BeginPlay() {
	for (Component* component : Components) {
		component->ComponentBeginPlay();
	}
}
void Actor::EndPlay() {
	for (Component* component : Components) {
		component->ComponentEndPlay();
	}
}
void Actor::Tick(float DeltaTime) {
	for (Component* component : Components) {
		component->ComponentTick(DeltaTime);
	}
}
void Actor::Destroy() {
	for (Component* component : Components) {
		component->ComponentDestroy();
	}
}
void Actor::PreFrameRendered() {
	for (Component* component : Components) {
		component->PreFrameRendered();
	}
}
void Actor::PostFrameRendered() {
	for (Component* component : Components) {
		component->PostFrameRendered();
	}
}
void Actor::DrawAdvanced(Camera* RenderCamera, EDrawType DrawType) {
	for (Component* component : Components) {
		component->DrawAdvanced(RenderCamera, DrawType);
	}
}

void Actor::RegisterComponent(Component* Component) {
	if (Components.size() == 0) {
		RootComponent = Component;
	}
	Components.push_back(Component);
	Component->OnRegisteredToActor(this);
}
bool Actor::DeregisterComponent(Component* Component) {
	for (int i = 0; i < Components.size(); i++) {
		if (Components[i] == Component) {
			Components.erase(Components.begin() + i);
			Component->OnDeregisteredFromActor();
			return true;
		}
	}
	return false;
}
SSet<Component*> Actor::GetComponents() {
	return Components;
}

bool Actor::PopulateDetailsPanel() {
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Transform", flags)) {
		for (BaseWidget* widget : DetailsPanelWidgets) {
			widget->Draw();
		}
	}
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Visibility", flags)) {
		ImGui::Checkbox("Visible", &bVisible);
	}
	return true;
}
TString Actor::GetDetailsPanelName() {
	return GetName();
}