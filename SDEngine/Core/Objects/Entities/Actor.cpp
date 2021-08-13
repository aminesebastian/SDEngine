#include "Actor.h"
#include "Core/Objects/Entities/Camera.h"
#include "Core/Objects/Entities/Component.h"
#include "Core/Objects/Entities/Entity.h"
#include "Actor.reflected.h"

Actor::Actor(const TString& Name) : Entity(Name) {
	RootComponent = nullptr;
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
void Actor::DrawAdvanced(const Camera* RenderCamera, const EDrawType& DrawType) {
	for (Component* component : Children) {
		if (component->ShouldBeDrawn(DrawType)) {
			component->DrawAdvanced(RenderCamera, DrawType);
		}
	}
}

const Vector3D Actor::GetWorldLocation() const {
	return GetLocation();
}
const Vector3D Actor::GetWorldRotation() const {
	return GetRotation();
}
const Vector3D Actor::GetWorldScale() const {
	return GetScale();
}
const FTransform Actor::GetWorldTransform() const {
	return GetTransform();
}
const FTransform Actor::GetLastFrameWorldTransform() const {
	return GetLastFrameTransform();
}

void Actor::RegisterComponent(Component* Component) {
	if (Children.empty()) {
		RootComponent = Component;
	}
	Children.push_back(Component);
	Component->SetOwner(this);
	Component->OnRegisteredToActor(this);
}
bool Actor::DeregisterComponent(Component* Component) {
	for (int i = 0; i < Children.size(); i++) {
		if (Children[i] == Component) {
			Children.erase(Children.begin() + i);
			Component->OnDeregisteredFromActor();
			Component->SetOwner(nullptr);
			return true;
		}
	}
	return false;
}
SSet<Component*> Actor::GetComponents() {
	return Children;
}
