#include "EngineObject.h"
#include "EngineObject.reflected.h"

EngineObject::EngineObject(const TString& Name) : EngineObjectName(Name) {}
EngineObject::~EngineObject() {}

const TString& EngineObject::GetObjectName() const {
	return EngineObjectName;
}
void EngineObject::OnPropertyUpdated(const FProperty* Property) {

}
void EngineObject::Destroy() {
	SD_ENGINE_DEBUG("Destroying Engine Object: {0}.", GetObjectName());
	OnDestroyed();
	OnDestroyedDelegate.Broadcast(this);
	this->~EngineObject();
	free(this);
}
void EngineObject::OnDestroyed() {

}