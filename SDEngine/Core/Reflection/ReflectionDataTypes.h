#pragma once
#include "Core/DataStructures/DataStructures.h"
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Reflection/ReflectionDataTypes.h"
#include "Core/Reflection/TypeDescriptor.h"

class EngineObject;

enum class ETypeMemoryType : uint8 {
	VALUE, REFERENCE, POINTER
};

struct FPropertyMetadata {
	FPropertyMetadata() {}
	FPropertyMetadata(const char* Category, bool bInspectorHidden) : Category(Category), bInspectorHidden(bInspectorHidden) {}

	const char* GetCategory() const {
		return Category;
	}
	const bool& GetHiddenInInspector() const {
		return bInspectorHidden;
	}
	bool operator==(const FPropertyMetadata& Other) const {
		if (bInspectorHidden == Other.bInspectorHidden) {
			if (Category == Other.Category) {
				return true;
			}
		}
		return false;
	}
private:
	friend struct TypeDescriptor_Struct;
	friend struct TypeDescriptor_Class;

	bool bInspectorHidden;
	const char* Category;
};
struct FProperty {
	FProperty(const char* Name, const char* InspectorName, const size_t& Offset, TypeDescriptor* Type, const FPropertyMetadata& Metadata) {
		this->Name = Name;
		this->InspectorName = InspectorName;
		this->Offset = Offset;
		this->Type = Type;
		this->Metadata = Metadata;
	}
	const TString GetName() const {
		return Name;
	}
	const TString GetInspectorName() const {
		return InspectorName;
	}
	const TypeDescriptor* GetType() const {
		return Type;
	}
	const FPropertyMetadata& GetMetadata() const {
		return Metadata;
	}
	template<typename T>
	const T* GetValue(const EngineObject* Object) const {
		return GetInternalValue(Object);
	}
	template<typename T>
	const T* GetValue(const void* Struct) const {
		return GetInternalValue<T>(Struct);
	}

	template<typename T>
	void SetValue(const EngineObject* Object, const T& Value) const {
		T* target = GetInternalValue<T>(Object);
		*target = Value;
	}
	template<typename T>
	void SetValue(const void* Struct, const T& Value) const {
		T* target = GetInternalValue<T>(Struct);
		*target = Value;
	}

	template<typename T>
	void SetValue(const EngineObject* Object, T* Value) const {
		T* target = GetInternalValue<T>(Object);
		target = Value;
	}
	template<typename T>
	void SetValue(const void* Struct, T* Value) const {
		T* target = GetInternalValue<T>(Struct);
		target = Value;
	}

	bool operator==(const FProperty& Other) const {
		if (Name == Other.Name) {
			if (InspectorName == Other.InspectorName) {
				if (Offset == Other.Offset) {
					if (Type == Other.Type) {
						if (Metadata == Other.Metadata) {
							return true;
						}
					}
				}
			}
		}
		return false;
	}

private:
	friend struct TypeDescriptor_Struct;
	friend struct TypeDescriptor_Class;

	const char* Name;
	const char* InspectorName;
	size_t Offset;
	TypeDescriptor* Type;
	FPropertyMetadata Metadata;

	template<typename T>
	T* GetInternalValue(const void* Object) const {
		return (T*)((char*)Object + Offset);
	}
};
struct FFunctionParameter {
	FFunctionParameter() { }
	FFunctionParameter(const char* Name, TypeDescriptor* Type, const bool& Const, const ETypeMemoryType& PassByType) {
		this->Name = Name;
		this->Type = Type;
		this->Const = Const;
		this->PassByType = PassByType;
	}
	const TString GetName() const {
		return Name;
	}
	const TypeDescriptor* GetType() const {
		return Type;
	}
	const bool& IsConst() const {
		return Const;
	}
	const ETypeMemoryType& GetPassByType() const {
		return PassByType;
	}
	bool operator==(const FFunctionParameter& Other) const {
		if (Name == Other.Name) {
			if (Type == Other.Type) {
				if (Const == Other.Const) {
					if (PassByType == Other.PassByType) {
						return true;
					}
				}
			}
		}
		return false;
	}
	bool operator!=(const FFunctionParameter& Other) const {
		return !(*this == Other);
	}

private:
	friend struct TypeDescriptor_Struct;
	friend struct TypeDescriptor_Class;

	const char* Name;
	TypeDescriptor* Type;
	bool Const;
	ETypeMemoryType PassByType;
};
struct FFunction {
	FFunction(const char* Name, const bool& ReturnConst, TypeDescriptor* bReturnConst, const ETypeMemoryType& ReturnMemoryType, const bool& bConstFunction, const std::initializer_list<FFunctionParameter>& InitializationList) {
		this->Name = Name;
		this->bReturnConst = bReturnConst;
		this->ReturnType = ReturnType;
		this->ReturnMemoryType = ReturnMemoryType;
		this->bConstFunction = bConstFunction;
		this->Parameters = InitializationList;
	}
	const TString GetName() const {
		return Name;
	}
	const bool& GetReturnsConst() const {
		return bReturnConst;
	}
	const TypeDescriptor* GetReturnType() const {
		return ReturnType;
	}
	const ETypeMemoryType& GetReturnMemoryType() const {
		return ReturnMemoryType;
	}
	const bool& IsConst() const {
		return bConstFunction;
	}
	const std::vector<FFunctionParameter>& GetParameters() const {
		return Parameters;
	}
	void AddParameter(FFunctionParameter Param) {
		Parameters.push_back(Param);
	}
	bool operator==(const FFunction& Other) const {
		if (Name == Other.Name) {
			if (bReturnConst == Other.bReturnConst) {
				if (ReturnType == Other.ReturnType) {
					if (ReturnMemoryType == Other.ReturnMemoryType) {
						if (bConstFunction == Other.bConstFunction) {
							if (Parameters.size() == Other.Parameters.size()) {
								for (int i = 0; i < Parameters.size(); i++) {
									if (Parameters[i] != Other.Parameters[i]) {
										return false;
									}
								}
								return true;
							}
						}
					}
				}
			}
		}
		return false;
	}

private:
	friend struct TypeDescriptor_Struct;
	friend struct TypeDescriptor_Class;

	const char* Name;
	bool bReturnConst;
	TypeDescriptor* ReturnType;
	ETypeMemoryType ReturnMemoryType;
	bool bConstFunction;
	std::vector<FFunctionParameter> Parameters;
};

