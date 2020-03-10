#pragma once
#include "Core/Reflection/Reflection.h"
#include "Core/Objects/EngineObject.h"

class ReflectionWrapper {
	ReflectionWrapper(const EngineObject* Object) {
		bIsObject = true;
		Target = (const void*)Object;
		Type = Object->StaticDescriptor();
	}
	ReflectionWrapper(const void* Struct, const TypeDescriptor* StructType) {
		bIsObject = false;
		Target = Struct;
		Type = StructType;
	}
	const bool& IsObject() const {
		return bIsObject;
	}
	const TypeDescriptor* GetType() const {
		return Type;
	}
	template<typename T> 
	T* GetPropertyValue(const FProperty* Property) {
		if (bIsObject) {
			return Property->GetValue<T>((EngineObject*)Target);
		} else {
			return Property->GetValue<T>(Target);
		}
	}
	template<typename T>
	void SetPropertyValue(const FProperty* Property, T* Value) {
		if (bIsObject) {
			Property->SetValue<T>((EngineObject*)Target, Value);
		} else {
			Property->SetValue<T>(Target, Value);
		}
	}
	template<typename T>
	void SetPropertyValue(const FProperty* Property, const T& Value) {
		if (bIsObject) {
			Property->SetValue<T>((EngineObject*)Target, Value);
		} else {
			Property->SetValue<T>(Target, Value);
		}
	}
private:
	bool bIsObject;
	const void* Target;
	const TypeDescriptor* Type;
};