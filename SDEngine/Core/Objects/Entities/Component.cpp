#include "Component.h"
#include "Core/Objects/Entities/Actor.h"

Component::Component(const TString& Name) : Entity(Name) {
	OwningActor = nullptr;
	OwningComponent = nullptr;
}
Component::~Component() {}

void Component::ComponentBeginPlay() {}
void Component::ComponentEndPlay() {}
void Component::ComponentTick(float DeltaTime) {}
void Component::ComponentDestroy() {}

void Component::OnRegisteredToActor(Actor* Owner) {
	this->OwningActor = Owner;
}
void Component::OnDeregisteredFromActor() {
	OwningActor = nullptr;
}
void Component::OnAttachedToComponent(Component* Component) {
	OwningComponent = Component;
}
void Component::OnDetatchedFromComponent() {
	OwningComponent = nullptr;
}
Actor* Component::GetOwningActor() {
	return OwningActor;
}

vec3 Component::GetWorldLocation() {
	if (OwningComponent) {
		return GetLocation() + OwningComponent->GetWorldLocation();
	}
	if (OwningActor) {
		return GetLocation() + OwningActor->GetWorldLocation();
	}
	return GetLocation();
}
vec3 Component::GetWorldRotation() {
	return GetRotation();
}
vec3 Component::GetWorldScale() {
	return GetScale();
}
Transform Component::GetWorldTransform() {
	if (OwningComponent) {
		return GetTransform() + OwningComponent->GetWorldTransform();
	}
	if (OwningActor) {
		return GetTransform() + OwningActor->GetWorldTransform();
	}
	return GetTransform();
}
Transform Component::GetLastFrameWorldTransform() {
	if (OwningComponent) {
		return GetLastFrameTransform() + OwningComponent->GetLastFrameWorldTransform();
	}
	if (OwningActor) {
		return GetLastFrameTransform() + OwningActor->GetLastFrameWorldTransform();
	}
	return GetLastFrameTransform();
}

void Component::AddChild(Component* Component) {
	Children.push_back(Component);
}
bool Component::RemoveChild(Component* Component) {
	for (int i = 0; i < Children.size(); i++) {
		if (Children[i] == Component) {
			Children.erase(Children.begin() + i);
			return true;
		}
	}
	return false;
}
SSet<Component*> Component::GetChildren(bool bAllDescendants) {
	if (bAllDescendants) {
		SSet<Component*> comps;
		GetAllChildrenRecursive(this, comps);
		return comps;
	} else {
		return Children;
	}
}
void Component::GetAllChildrenRecursive(Component* Root, SSet<Component*>& Components) {

}

void Component::PostFrameRendered() {
	Entity::PostFrameRendered();
	SetLastFrameTransform(GetTransform());
}