#include "Entity.h"
#include "Core/Objects/Entities/Camera.h"
#include "Core/Engine/World.h"

Entity::Entity(const TString& Name, const TString& Type) : EngineObject(Name, Type) {
	bVisible        = true;
	bHiddenInGame   = false;
	bNeedsTick      = true;
	bNeedsBeginPlay = true;
	bCastShadows    = true;
	OwningWorld     = nullptr;
	Owner           = nullptr;
}
Entity::~Entity() {};

bool Entity::IsVisible() const {
	return bVisible;
}
void Entity::SetVisibility(bool Show) {
	bVisible = Show;
}
void Entity::ToggleVisibility() {
	bVisible = !bVisible;
}
bool Entity::IsHiddenInGame() const {
	return bHiddenInGame;
}
void Entity::SetHiddenInGame(bool Hidden) {
	bHiddenInGame = Hidden;
}

bool Entity::ShouldCastShadows() const {
	return bCastShadows;
}
void Entity::SetCastShadows(bool CastShadows) {
	bCastShadows = CastShadows;
}


bool Entity::ShouldBeDrawn(EDrawType DrawType) {
	if (DrawType == SCENE_RENDER || DrawType == EDITOR_OUTLINE_RENDER) {
		if (IsVisible() && !IsHiddenInGame()) {
			return true;
		}
		if (IsVisible() && IsHiddenInGame() && !GetWorld()->IsInGameMode()) {
			return true;
		}
	} else if (DrawType == SHADOW_MAP_RENDER) {
		return IsVisible() && ShouldCastShadows();
	}
	return false;
}

const Transform& Entity::GetTransform() const {
	return CurrentTransform;
}
void Entity::SetTransform(const Transform& NewTransform) {
	CurrentTransform = NewTransform;
}
const Transform& Entity::GetLastFrameTransform() const {
	return LastFrameTrasnform;
}
void Entity::SetLastFrameTransform(const Transform& OldTransform) {
	LastFrameTrasnform = OldTransform;
}
vec3 Entity::GetLinearVelocity() const {
	return CurrentTransform.GetLocation() - LastFrameTrasnform.GetLocation();
}
vec3 Entity::GetAngularVelocity() const {
	return CurrentTransform.GetRotation() - LastFrameTrasnform.GetRotation();
}

const vec3& Entity::GetLocation() const {
	return CurrentTransform.GetLocation();
}
const vec3& Entity::GetRotation() const {
	return CurrentTransform.GetRotation();
}
const vec3& Entity::GetScale() const {
	return CurrentTransform.GetScale();
}

void Entity::AddLocation(vec3 Offset) {
	CurrentTransform.AddLocation(Offset);
}
void Entity::AddLocation(float X, float Y, float Z) {
	CurrentTransform.AddLocation(vec3(X, Y, Z));
}
void Entity::SetLocation(vec3 Location) {
	CurrentTransform.SetLocation(Location);
}
void Entity::SetLocation(float X, float Y, float Z) {
	CurrentTransform.SetLocation(vec3(X, Y, Z));
}

void Entity::AddRotation(vec3 RotationDelta) {
	CurrentTransform.AddRotation(RotationDelta);
}
void Entity::AddRotation(float X, float Y, float Z) {
	CurrentTransform.AddRotation(vec3(X, Y, Z));
}
void Entity::SetRotation(vec3 Rotation) {
	CurrentTransform.SetRotation(Rotation);
}
void Entity::SetRotation(float X, float Y, float Z) {
	CurrentTransform.SetRotation(vec3(X, Y, Z));
}

void Entity::AddScale(vec3 ScaleDelta) {
	CurrentTransform.AddScale(ScaleDelta);
}
void Entity::AddScale(float X, float Y, float Z) {
	CurrentTransform.AddScale(vec3(X, Y, Z));
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
void Entity::SetOwner(Entity* OwnerIn) {
	Owner = OwnerIn;
}
Entity* Entity::GetOwner() {
	return Owner;
}
void Entity::PreFrameRendered() {

}
void Entity::PostFrameRendered() {
	SetLastFrameTransform(CurrentTransform);
}
void Entity::Draw(const Camera* RenderCamera) {
	DrawAdvanced(RenderCamera, SCENE_RENDER);
}

void Entity::OnAddedToWorld(World* WorldIn) {
	OwningWorld = WorldIn;
}
void Entity::OnRemovedFromWorld() {
	OwningWorld = nullptr;
}
const World* Entity::GetWorld() const {
	if (Owner) {
		return Owner->GetWorld();
	}
	return OwningWorld;
}