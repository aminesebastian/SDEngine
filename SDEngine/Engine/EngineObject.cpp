#include "EngineObject.h"

EngineObject::EngineObject(const TString& Name) {
	this->Name = Name;
}
EngineObject::~EngineObject() {

}

TString EngineObject::GetName() {
	return Name;
}