#include "Component.h"

Component::Component(const TString& Name) : Entity(Name) {
	Owner = nullptr;
}
Component::~Component() {}

void Component::ComponentBeginPlay() {}
void Component::ComponentEndPlay() {}
void Component::ComponentTick(float DeltaTime) {}
void Component::ComponentDestroy() {}

void Component::OnRegisteredToActor(Actor* Owner) {
	this->Owner = Owner;
}
void Component::OnDeregisteredFromActor() {
	Owner = nullptr;
}
Actor* Component::GetOwner() {
	return Owner;
}