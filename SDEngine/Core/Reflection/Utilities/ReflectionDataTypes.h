#pragma once
#include "Core/DataStructures/DataStructures.h"
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Reflection/Utilities/ReflectionDataTypes.h"
#include "Core/Reflection/Utilities/TypeDescriptor.h"

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
	friend class TypeDescriptor_Struct;
	friend class TypeDescriptor_Class;

	bool bInspectorHidden;
	const char* Category;
};
struct FProperty {
	FProperty(const char* Name, const char* InspectorName, const size_t& Offset, ITypeDescriptor* Type, const FPropertyMetadata& Metadata) {
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
	const ITypeDescriptor* GetType() const {
		return Type;
	}
	const FPropertyMetadata& GetMetadata() const {
		return Metadata;
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
	friend class TypeDescriptor_Struct;
	friend class TypeDescriptor_Class;
	friend class PropertyHandle;

	const char* Name;
	const char* InspectorName;
	size_t Offset;
	ITypeDescriptor* Type;
	FPropertyMetadata Metadata;

	template<typename T>
	const T* GetValue(const void* Target) const {
		return GetInternalValue<T>(Target);
	}
	template<typename T>
	const T* GetValue(void* Target) const {
		return GetInternalValue<T>(Target);
	}
	template<typename T>
	void SetValue(const void* Target, const T& Value) const {
		T* target = GetInternalValue<T>(Target);
		*target = Value;
	}

	template<typename T>
	void SetValue(const void* Target, T* Value) const {
		T* target = GetInternalValue<T>(Struct);
		target = Value;
	}
	template<typename T>
	T* GetInternalValue(const void* Target) const {
		return (T*)((char*)Target + Offset);
	}
	template<typename T>
	T* GetInternalValue(void* Target) const {
		return (T*)((char*)Target + Offset);
	}
};
struct FFunctionParameter {
	FFunctionParameter() { }
	FFunctionParameter(const char* Name, ITypeDescriptor* Type, const bool& Const, const ETypeMemoryType& PassByType) {
		this->Name = Name;
		this->Type = Type;
		this->Const = Const;
		this->PassByType = PassByType;
	}
	const TString GetName() const {
		return Name;
	}
	const ITypeDescriptor* GetType() const {
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
	friend class TypeDescriptor_Struct;
	friend class TypeDescriptor_Class;
	friend class ReflectionHandle;

	const char* Name;
	ITypeDescriptor* Type;
	bool Const;
	ETypeMemoryType PassByType;
};
struct FFunction {
	FFunction(const char* Name, const bool& ReturnConst, ITypeDescriptor* bReturnConst, const ETypeMemoryType& ReturnMemoryType, const bool& bConstFunction, const std::initializer_list<FFunctionParameter>& InitializationList) {
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
	const ITypeDescriptor* GetReturnType() const {
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
	friend class TypeDescriptor_Struct;
	friend class TypeDescriptor_Class;
	friend class ReflectionHandle;

	const char* Name;
	bool bReturnConst;
	ITypeDescriptor* ReturnType;
	ETypeMemoryType ReturnMemoryType;
	bool bConstFunction;
	std::vector<FFunctionParameter> Parameters;
};

