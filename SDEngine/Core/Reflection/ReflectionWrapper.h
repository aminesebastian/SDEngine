#pragma once
#include "Core/Reflection/Reflection.h"
#include "Core/Objects/EngineObject.h"

class ReflectionWrapper {
public:
	ReflectionWrapper() {
		Target = nullptr;
		Type = nullptr;
		bIsValid = false;
		bIsObject = false;
	}
	ReflectionWrapper(const EngineObject* Object) : ReflectionWrapper() {
		if (Object) {
			bIsValid = true;
			bIsObject = true;
			Target = (const void*)Object;
			Type = Object->StaticDescriptor();
		}
	}
	ReflectionWrapper(const void* Struct, const TypeDescriptor* StructType): ReflectionWrapper(){
		if (Struct) {
			bIsValid = true;
			bIsObject = false;
			Target = Struct;
			Type = StructType;
		}
	}
	const bool& IsObject() const {
		return bIsObject;
	}
	const TypeDescriptor* GetType() const {
		return Type;
	}
	const bool& IsValid() const {
		return bIsValid;
	}
	template<typename T>
	const T* Get() const {
		return (T*)Target;
	}
	template<typename T>
	const T* GetPropertyValue(const FProperty* Property) const {
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
	bool bIsValid;
	bool bIsObject;
	const void* Target;
	const TypeDescriptor* Type;
};