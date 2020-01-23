#include "Entity.h"
#include "Entities/Camera.h"
#include "UserInterface/UserInterface.h"
#include "UserInterface/UserInterfaceUtilities.h"
#include "UserInterface/Widgets/DragFloat.h"

Entity::Entity(TString Name) : EngineObject(Name) {
	bVisible = true;
	bNeedsTick = true;
	bNeedsBeginPlay = true;
}
Entity::~Entity() {};

Transform Entity::GetTransform() { return CurrentTransform; }
void Entity::SetTransform(Transform NewTransform) {
	CurrentTransform = NewTransform;
}
Transform Entity::GetLastFrameTransform() { return LastFrameTransform; }
void Entity::SetLastFrameTransform(Transform OldTransform) {
	LastFrameTransform = OldTransform;
}
vec3 Entity::GetLinearVelocity() {
	return CurrentTransform.GetLocation() - LastFrameTransform.GetLocation();
}
vec3 Entity::GetAngularVelocity() {
	return CurrentTransform.GetRotation() - LastFrameTransform.GetRotation();
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
	CurrentTransform.GetLocation() += Offset;
}
void Entity::AddLocation(float X, float Y, float Z) {
	CurrentTransform.GetLocation() += vec3(X, Y, Z);
}
void Entity::SetLocation(vec3 Location) {
	CurrentTransform.SetLocation(Location);
}
void Entity::SetLocation(float X, float Y, float Z) {
	CurrentTransform.SetLocation(vec3(X, Y, Z));
}

void Entity::AddRotation(vec3 RotationDelta) {
	CurrentTransform.GetRotation() += RotationDelta;
}
void Entity::AddRotation(float X, float Y, float Z) {
	CurrentTransform.GetRotation() += vec3(X, Y, Z);
}
void Entity::SetRotation(vec3 Rotation) {
	CurrentTransform.SetRotation(Rotation);
}
void Entity::SetRotation(float X, float Y, float Z) {
	CurrentTransform.SetRotation(vec3(X, Y, Z));
}

void Entity::AddScale(vec3 ScaleDelta) {
	CurrentTransform.GetScale() += ScaleDelta;
}
void Entity::AddScale(float X, float Y, float Z) {
	CurrentTransform.GetScale() += vec3(X, Y, Z);
}
void Entity::SetScale(vec3 Scale) {
	CurrentTransform.SetScale(Scale);
}
void Entity::SetScale(float X, float Y, float Z) {
	CurrentTransform.SetScale(vec3(X, Y, Z));
}
void Entity::SetUniformScale(float Scale) {
	CurrentTransform.SetScale(vec3(Scale, Scale, Scale));
}

bool Entity::IsVisible() { return bVisible; }
void Entity::SetVisibility(bool Show) { bVisible = Show; }
void Entity::ToggleVisibility() { bVisible = !bVisible; }

void Entity::PreFrameRendered() {

}
void Entity::PostFrameRendered() {
	SetLastFrameTransform(CurrentTransform);
}
void Entity::Draw(Camera* RenderCamera) {
	DrawAdvanced(RenderCamera, SCENE_RENDER);
}