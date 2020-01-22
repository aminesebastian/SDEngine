#include "Entity.h"
#include "UserInterface/UserInterface.h"
#include "UserInterface/UserInterfaceUtilities.h"
#include "UserInterface/Widgets/DragFloat.h"

Entity::Entity(TString Name, const Transform SpawnTransform) :
	S_Name(Name),
	S_Transform(SpawnTransform),
	S_LastFrameTransform(SpawnTransform),
	S_InitialTransform(SpawnTransform),
	S_AABB(nullptr),
	S_World(nullptr) {
	DragFloat* location = new DragFloat("Location");
	location->AddEntry(&S_Transform.GetLocation().x, "X %.3f", FColor(1.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.01f, 2.0f);
	location->AddEntry(&S_Transform.GetLocation().y, "Y %.3f", FColor(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 0.01f, 2.0f);
	location->AddEntry(&S_Transform.GetLocation().z, "Z %.3f", FColor(0.0f, 0.0f, 1.0f), 0.0f, 0.0f, 0.01f, 2.0f);
	DetailsPanelWidgets.push_back(location);

	std::function<float(float)> toDegrees = ([](float radians) { return glm::degrees(radians); });
	std::function<float(float)> toRadians = ([](float degrees) { return glm::radians(degrees); });

	DragFloat* rotation = new DragFloat("Rotation");
	FDragFloatEntry* x = rotation->AddEntry(&S_Transform.GetRotation().x, "P %.3f", FColor(1.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.1f, 2.0f);
	x->OnFormatForViewFunction = toDegrees;
	x->OnFormatFromView = toRadians;

	FDragFloatEntry* y = rotation->AddEntry(&S_Transform.GetRotation().y, "Y %.3f", FColor(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 0.1f, 2.0f);
	y->OnFormatForViewFunction = toDegrees;
	y->OnFormatFromView = toRadians;

	FDragFloatEntry* z = rotation->AddEntry(&S_Transform.GetRotation().z, "R %.3f", FColor(0.0f, 0.0f, 1.0f), 0.0f, 0.0f, 0.1f, 2.0f);
	z->OnFormatForViewFunction = toDegrees;
	z->OnFormatFromView = toRadians;

	DetailsPanelWidgets.push_back(rotation);

	DragFloat* scale = new DragFloat("Scale");
	scale->AddEntry(&S_Transform.GetScale().x, "X %.3f", FColor(1.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.01f, 2.0f);
	scale->AddEntry(&S_Transform.GetScale().y, "Y %.3f", FColor(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 0.01f, 2.0f);
	scale->AddEntry(&S_Transform.GetScale().z, "Z %.3f", FColor(0.0f, 0.0f, 1.0f), 0.0f, 0.0f, 0.01f, 2.0f);
	DetailsPanelWidgets.push_back(scale);
}

Entity::Entity(TString Name) : Entity(Name, Transform()) {}
Entity::~Entity() {};

void Entity::Draw(Camera* Camera, bool bCallerProvidesShader) {}
bool Entity::PopulateDetailsPanel() {
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
TString Entity::GetDetailsPanelName() {
	return GetName();
}

Transform Entity::GetTransform() { return S_Transform; }
Transform Entity::GetInitialTransform() { return S_InitialTransform; }
Transform Entity::GetLastFrameTransform() { return S_LastFrameTransform; }
vec3 Entity::GetLinearVelocity() {
	return S_Transform.GetLocation() - S_LastFrameTransform.GetLocation();
}
vec3 Entity::GetAngularVelocity() {
	return S_Transform.GetRotation() - S_LastFrameTransform.GetRotation();
}

vec3 Entity::GetLocation() {
	return GetTransform().GetLocation();
}
vec3 Entity::GetRotation() {
	return GetTransform().GetRotation();
}
vec3 Entity::GetScale() {
	return GetTransform().GetScale();
}

void Entity::AddLocation(vec3 Offset) {
	S_Transform.GetLocation() += Offset;
}
void Entity::AddLocation(float X, float Y, float Z) {
	S_Transform.GetLocation() += vec3(X, Y, Z);
}
void Entity::SetLocation(vec3 Location) {
	S_Transform.SetLocation(Location);
}
void Entity::SetLocation(float X, float Y, float Z) {
	S_Transform.SetLocation(vec3(X, Y, Z));
}

void Entity::AddRotation(vec3 RotationDelta) {
	S_Transform.GetRotation() += RotationDelta;
}
void Entity::AddRotation(float X, float Y, float Z) {
	S_Transform.GetRotation() += vec3(X, Y, Z);
}
void Entity::SetRotation(vec3 Rotation) {
	S_Transform.SetRotation(Rotation);
}
void Entity::SetRotation(float X, float Y, float Z) {
	S_Transform.SetRotation(vec3(X, Y, Z));
}

void Entity::AddScale(vec3 ScaleDelta) {
	S_Transform.GetScale() += ScaleDelta;
}
void Entity::AddScale(float X, float Y, float Z) {
	S_Transform.GetScale() += vec3(X, Y, Z);
}
void Entity::SetScale(vec3 Scale) {
	S_Transform.SetScale(Scale);
}
void Entity::SetScale(float X, float Y, float Z) {
	S_Transform.SetScale(vec3(X, Y, Z));
}
void Entity::SetUniformScale(float Scale) {
	S_Transform.SetScale(vec3(Scale, Scale, Scale));
}

void Entity::SetTransform(Transform NewTransform) { S_Transform = NewTransform; }

bool Entity::IsVisible() { return bVisible; }
void Entity::UpdateVisibility(bool bShow) { bVisible = bShow; }
void Entity::ToggleVisibility() { bVisible = !bVisible; }
void Entity::CompleteSpawn(UWorld* World) { S_World = World; }
bool Entity::NeedsTick() { return bNeedsTick; }
bool Entity::NeedsBeginPlay() { return bNeedsBeginPlay; }

TString Entity::GetName() { return S_Name; }

void Entity::BeginPlay() {}
void Entity::Tick(float DeltaTime) {

}
void Entity::PreFrameRendered() {

}
void Entity::PostFrameRendered() {
	S_LastFrameTransform = S_Transform;
}
void Entity::Destroy() {
	delete this;
}
bool Entity::TraceAgainstRay(vec3 Origin, vec3 Direction, vec3& HitPoint, float& Distance, ECollisionChannel Channel) {
	return false;
}

AxisAlignedBoundingBox* Entity::GetAxisAlignedBoundingBox() { return S_AABB; }
bool Entity::HasAxisAlignedBoundingBox() { return S_AABB != NULL; }