#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Reflection/Reflection.h"

class ReflectionHelpers {
public:
	static const void GetPropertiesOfClass(SArray<FProperty*>& Properties, const class EngineObject* Object);

	static const FProperty* GetPropertyHandle(const TString& Name, const class EngineObject* Object) {
		SArray<FProperty*> properties;
		GetPropertiesOfClass(properties, Object);
		for (FProperty* prop : properties) {
			if (prop->GetName() == Name) {
				return prop;
			}
		}
		return 
			nullptr;
	}
	template<typename T>
	static const FProperty* GetPropertyHandleFromStruct(const TString& Name, const T* Struct) {
		TypeDescriptor_Struct* structType = Cast<TypeDescriptor_Struct>(TypeResolver<T>::Get());
		for (FProperty& prop : structType->Properties) {
			if (prop.GetName() == Name) {
				return &prop;
			}
		}
		return nullptr;
	}
private:
	static void worker_GetAllMembersOfClass(const TypeDescriptor_Class* CurrentNode, SArray<FProperty*>& Output);
};

