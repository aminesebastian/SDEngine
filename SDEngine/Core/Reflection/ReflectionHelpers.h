#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Reflection/ReflectableTypes.h"
#include "Core/Reflection/TypeResolver.h"

class ReflectionHelpers {
public:
	static const void GetAllMembersOfClass(SArray<FProperty>& Properties, const class EngineObject* Object);
	template<typename T>
	static T* GetProperty(const TString& Name, const class EngineObject* Object) {
		SArray<FProperty> properties;
		GetAllMembersOfClass(properties, Object);
		for (FProperty& prop : properties) {
			if (prop.Name == Name) {
				return (T*)((char*)Object + prop.Offset);
			}
		}
		return nullptr;
	}
	template<typename T>
	static T* GetProperty(const FProperty& Property, const class EngineObject* Object) {
		return (T*)((char*)Object + Property.Offset);
	}
	template<typename T, typename K>
	static T* GetProperty(const TString& Name, const K* Struct) {
		TypeDescriptor_Struct* structType = Cast<TypeDescriptor_Struct>(TypeResolver<K>::Get());
		for (FProperty& prop : structType->Properties) {
			if (prop.Name == Name) {
				return (T*)((char*)Struct + prop.Offset);
			}
		}
		return nullptr;
	}
	template<typename T>
	static T* GetProperty(const FProperty& Property, const void* Struct) {
		return (T*)((char*)Struct + Property.Offset);
	}
private:
	static void worker_GetAllMembersOfClass(const TypeDescriptor_Class* CurrentNode, SArray<FProperty>& Output);
};

