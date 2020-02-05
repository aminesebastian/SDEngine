#include "EngineObject.h"

EngineObject::EngineObject(const TString& Name) : Name(Name) {

}
EngineObject::~EngineObject() {

}

const TString& EngineObject::GetName() const {
	return Name;
}