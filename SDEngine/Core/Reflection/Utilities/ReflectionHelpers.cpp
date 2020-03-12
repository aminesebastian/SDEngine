#include "ReflectionHelpers.h"
#include "Core/Objects/EngineObject.h"

const void ReflectionHelpers::GetPropertiesOfClass(SArray<const FProperty*>& Properties, const EngineObject* Object) {
	worker_GetAllMembersOfClass(Object->StaticDescriptor(), Properties);
}
void ReflectionHelpers::worker_GetAllMembersOfClass(const TypeDescriptor_Class* CurrentNode, SArray<const FProperty*>& Output) {
	// Add the members of the current node.
	for (const FProperty* property : CurrentNode->GetProperties()) {
		Output.Add(property);
	}

	// Continue down the tree.
	for (const TypeDescriptor_Class* desc : CurrentNode->GetParentClasses()) {
		if (desc) {
			worker_GetAllMembersOfClass(desc, Output);
		}
	}
}
const FProperty* ReflectionHelpers::GetProperty(const TString& Name, const class EngineObject* Object) {
	if (Object) {
		SArray<const FProperty*> properties;
		GetPropertiesOfClass(properties, Object);
		for (const FProperty* prop : properties) {
			if (prop->GetName() == Name) {
				return prop;
			}
		}
	}
	return nullptr;
}
const FProperty* ReflectionHelpers::GetProperty(const TString& Name, const void* Struct, const TypeDescriptor_Struct* Type) {
	if (Type && Struct) {
		for (const FProperty* prop : Type->GetProperties()) {
			if (prop->GetName() == Name) {
				return prop;
			}
		}
	}
	return nullptr;
}