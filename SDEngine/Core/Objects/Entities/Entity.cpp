#include "Entity.h"
#include "Core/Objects/Entities/Camera.h"
#include "Core/Engine/World.h"
#include "Entity.reflected.h"

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
void Entity::SetVisibility(const bool& Show) {
	bVisible = Show;
}
void Entity::ToggleVisibility() {
	bVisible = !bVisible;
}
bool Entity::IsHiddenInGame() const {
	return bHiddenInGame;
}
void Entity::SetHiddenInGame(const bool& Hidden) {
	bHiddenInGame = Hidden;
}

bool Entity::CastsShadows() const {
	return bCastShadows;
}
void Entity::SetCastShadows(const bool& CastShadows) {
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
		return IsVisible() && CastsShadows();
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
	return LastFrameTransform;
}
void Entity::SetLastFrameTransform(const Transform& OldTransform) {
	LastFrameTransform = OldTransform;
}
const Vector3D Entity::GetLinearVelocity() const {
	return CurrentTransform.GetLocation() - LastFrameTransform.GetLocation();
}
const Vector3D Entity::GetAngularVelocity() const {
	return CurrentTransform.GetRotation() - LastFrameTransform.GetRotation();
}

const Vector3D& Entity::GetLocation() const {
	return CurrentTransform.GetLocation();
}
const Vector3D& Entity::GetRotation() const {
	return CurrentTransform.GetRotation();
}
const Vector3D& Entity::GetScale() const {
	return CurrentTransform.GetScale();
}

void Entity::AddLocation(Vector3D Offset) {
	CurrentTransform.AddLocation(Offset);
}
void Entity::AddLocation(const float& X, const float& Y, const float& Z) {
	CurrentTransform.AddLocation(Vector3D(X, Y, Z));
}
void Entity::SetLocation(const Vector3D& Location) {
	CurrentTransform.SetLocation(Location);
}
void Entity::SetLocation(const float& X, const float& Y, const float& Z) {
	CurrentTransform.SetLocation(Vector3D(X, Y, Z));
}

void Entity::AddRotation(const Vector3D& RotationDelta) {
	CurrentTransform.AddRotation(RotationDelta);
}
void Entity::AddRotation(const float& X, const float& Y, const float& Z) {
	CurrentTransform.AddRotation(Vector3D(X, Y, Z));
}
void Entity::SetRotation(const Vector3D& Rotation) {
	CurrentTransform.SetRotation(Rotation);
}
void Entity::SetRotation(const float& X, const float& Y, const float& Z) {
	CurrentTransform.SetRotation(Vector3D(X, Y, Z));
}

void Entity::AddScale(const Vector3D& ScaleDelta) {
	CurrentTransform.AddScale(ScaleDelta);
}
void Entity::AddScale(const float& X, const float& Y, const float& Z) {
	CurrentTransform.AddScale(Vector3D(X, Y, Z));
}
void Entity::SetScale(const Vector3D& Scale) {
	CurrentTransform.SetScale(Scale);
}
void Entity::SetScale(const float& X, const float& Y, const float& Z) {
	CurrentTransform.SetScale(Vector3D(X, Y, Z));
}
void Entity::SetUniformScale(const float& Scale) {
	CurrentTransform.SetScale(Vector3D(Scale, Scale, Scale));
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