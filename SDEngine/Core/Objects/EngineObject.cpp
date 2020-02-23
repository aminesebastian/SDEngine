#include "EngineObject.h"

EngineObject::EngineObject(const TString& Name, const TString& Type) : EngineObjectName(Name), EngineObjectType(Type) {}
EngineObject::~EngineObject() {}

const TString& EngineObject::GetObjectName() const {
	return EngineObjectName;
}
const TString& EngineObject::GetObjectType() const {
	return EngineObjectType;
}
void EngineObject::SetObjectType(const TString& Type) {
	EngineObjectType = Type;
}
