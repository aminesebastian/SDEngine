#pragma once
#include "Core/DataStructures/DataStructures.h"
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Reflection/Utilities/TypeDescriptor.h"
#include "Core/Reflection/Utilities/ReflectionDataTypes.h"

class TypeDescriptor_Struct : public ITypeDescriptor {
public:
	TypeDescriptor_Struct(const char* name, size_t size, void (*InitializationFunction)(TypeDescriptor_Struct*)) : ITypeDescriptor("", 0, EPrimitiveDataType::Struct) {
		InitializationFunction(this);
	}
	virtual const TString ToString(const void* Instance, const int32 Indent = 0) const override {
		TString result = Name + " {" + '\n';
		for (const FProperty* member : Properties) {
			TString memberValue = member->GetType()->ToString((char*)Instance + member->Offset, Indent + 1);
			result += std::string(4 * ((int64)Indent + 1), ' ') + member->GetName() + TString(" = ") + memberValue;
			result += '\n';
		}
		result += std::string(4 * (int64)Indent, ' ') + "}";
		return result;
	}
	
	/**
	* Gets all the reflected properties of this struct.
	*
	* @returns	A vector of the properties of this struct.
	*/
	const std::vector<const FProperty*> GetProperties() const {
		return Properties;
	}

	void AddProperty(const FProperty* Property) {
		Properties.push_back(Property);
	}

private:
	std::vector<const FProperty*> Properties;
};

class TypeDescriptor_Class : public ITypeDescriptor {
public:
	TypeDescriptor_Class(const char* Name, size_t Size, void (*InitializationFunction)(TypeDescriptor_Class*)) : ITypeDescriptor(Name, Size, EPrimitiveDataType::Class) {
		InitializationFunction(this);
	}
	virtual const TString ToString(const void* Instance, const int32 Indent = 0) const override {
		TString result = Name + " {" + '\n';
		for (const FProperty* member : Properties) {
			TString memberValue = member->GetType()->ToString((char*)Instance + member->Offset, Indent + 1);
			result += std::string(4 * ((int64)Indent + 1), ' ') + member->GetName() + TString(" = ") + memberValue;
			result += '\n';
		}
		result += std::string(4 * (int64)Indent, ' ') + "}";
		return result;
	}

	/**
	 * Gets all the reflected properties of this class.
	 *
	 * @returns	A vector of the properties of this class.
	 */
	const std::vector<const FProperty*> GetProperties() const {
		return Properties;
	}
	/**
	* Gets all the reflected functions of this class.
	*
	* @returns	A vector of the functions of this class.
	*/
	const std::vector<const FFunction*> GetFunctions() const {
		return Functions;
	}
	/**
	* Gets all the reflected parent classes of this class.
	*
	* @returns	A vector of the parent classes of this class.
	*/
	const std::vector<const TypeDescriptor_Class*> GetParentClasses() const {
		return ParentDescriptors;
	}

	void AddProperty(const FProperty* Property) {
		Properties.push_back(Property);
	}
	void AddFunction(const FFunction* Function) {
		Functions.push_back(Function);
	}
	void AddParent(const TypeDescriptor_Class* ParentClass) {
		ParentDescriptors.push_back(ParentClass);
	}
private:
	std::vector<const FProperty*> Properties;
	std::vector<const FFunction*> Functions;
	std::vector<const TypeDescriptor_Class*> ParentDescriptors;
};

