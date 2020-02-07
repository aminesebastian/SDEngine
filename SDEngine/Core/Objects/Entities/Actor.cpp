#include "Actor.h"
#include "Core/Objects/Entities/Camera.h"
#include "Core/Objects/Entities/Component.h"
#include "Core/Objects/Entities/Entity.h"
#include "UserInterface/UserInterface.h"
#include "UserInterface/UserInterfaceUtilities.h"
#include "UserInterface/Widgets/DragFloat.h"

Actor::Actor(const TString& Name) : Entity(Name) {
	RootComponent = nullptr;
	DragFloat* location = new DragFloat("Location");
	location->AddEntry(&CurrentTransform.GetLocation().x, "X %.3f", FColor(1.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.01f, 2.0f);
	location->AddEntry(&CurrentTransform.GetLocation().y, "Y %.3f", FColor(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 0.01f, 2.0f);
	location->AddEntry(&CurrentTransform.GetLocation().z, "Z %.3f", FColor(0.0f, 0.0f, 1.0f), 0.0f, 0.0f, 0.01f, 2.0f);
	DetailsPanelWidgets.Add(location);

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

	DetailsPanelWidgets.Add(rotation);

	DragFloat* scale = new DragFloat("Scale");
	scale->AddEntry(&CurrentTransform.GetScale().x, "X %.3f", FColor(1.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.01f, 2.0f);
	scale->AddEntry(&CurrentTransform.GetScale().y, "Y %.3f", FColor(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 0.01f, 2.0f);
	scale->AddEntry(&CurrentTransform.GetScale().z, "Z %.3f", FColor(0.0f, 0.0f, 1.0f), 0.0f, 0.0f, 0.01f, 2.0f);
	DetailsPanelWidgets.Add(scale);

}
Actor::~Actor() {}

void Actor::BeginPlay() {
	for (Component* component : Children) {
		component->ComponentBeginPlay();
	}
}
void Actor::EndPlay() {
	for (Component* component : Children) {
		component->ComponentEndPlay();
	}
}
void Actor::Tick(float DeltaTime) {
	for (Component* component : Children) {
		component->ComponentTick(DeltaTime);
	}
}
void Actor::Destroy() {
	for (Component* component : Children) {
		component->ComponentDestroy();
	}
}
void Actor::PreFrameRendered() {
	for (Component* component : Children) {
		component->PreFrameRendered();
	}
}
void Actor::PostFrameRendered() {
	Entity::PostFrameRendered();
	SetLastFrameTransform(GetTransform());
	for (Component* component : Children) {
		component->PostFrameRendered();
	}
}
void Actor::DrawAdvanced(Camera* RenderCamera, EDrawType DrawType) {
	for (Component* component : Children) {
		if (component->ShouldBeDrawn(DrawType)) {
			component->DrawAdvanced(RenderCamera, DrawType);
		}
	}
}

vec3 Actor::GetWorldLocation() {
	return GetLocation();
}
vec3 Actor::GetWorldRotation() {
	return GetRotation();
}
vec3 Actor::GetWorldScale() {
	return GetScale();
}
Transform Actor::GetWorldTransform() {
	return GetTransform();
}
Transform Actor::GetLastFrameWorldTransform() {
	return GetLastFrameTransform();
}

void Actor::RegisterComponent(Component* Component) {
	if (Children.size() == 0) {
		RootComponent = Component;
	}
	Children.push_back(Component);
	Component->OnRegisteredToActor(this);
}
bool Actor::DeregisterComponent(Component* Component) {
	for (int i = 0; i < Children.size(); i++) {
		if (Children[i] == Component) {
			Children.erase(Children.begin() + i);
			Component->OnDeregisteredFromActor();
			return true;
		}
	}
	return false;
}
SSet<Component*> Actor::GetComponents() {
	return Children;
}

bool Actor::PopulateDetailsPanel() {
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Transform", flags)) {
		for (DragFloat* widget : DetailsPanelWidgets) {
			widget->Draw();
		}
	}
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Properties", flags)) {
		ImGui::Checkbox("Visible", &bVisible);
		ImGui::Checkbox("Casts Shadows", &bCastShadows);
		ImGui::Checkbox("Hidden in Game", &bHiddenInGame);
	}
	return true;
}
TString Actor::GetDetailsPanelName() {
	return GetName();
}