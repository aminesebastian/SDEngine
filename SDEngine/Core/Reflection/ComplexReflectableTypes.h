#pragma once
#include "Core/DataStructures/DataStructures.h"
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Reflection/ReflectionDataTypes.h"
#include "Core/Reflection/TypeDescriptor.h"

struct TypeDescriptor_Struct : public TypeDescriptor {
	std::vector<FProperty> Properties;

	TypeDescriptor_Struct(void (*InitializationFunction)(TypeDescriptor_Struct*)) : TypeDescriptor{ "", 0 } {
		InitializationFunction(this);
	}
	TypeDescriptor_Struct(const char* name, size_t size, const std::initializer_list<FProperty>& InitializationList) : TypeDescriptor{ "", 0 }, Properties{ InitializationList } {

	}
	virtual const TString ToString(const void* Instance, const int32 Indent = 0) const override {
		TString result = Name + " {" + '\n';
		for (const FProperty& member : Properties) {
			TString memberValue = member.GetType()->ToString((char*)Instance + member.Offset, Indent + 1);
			result += std::string(4 * ((int64)Indent + 1), ' ') + member.GetName() + TString(" = ") + memberValue;
			result += '\n';
		}
		result += std::string(4 * (int64)Indent, ' ') + "}";
		return result;
	}
};

struct TypeDescriptor_Class : public TypeDescriptor {
	std::vector<FProperty> Properties;
	std::vector<FFunction> Functions;
	std::vector<TypeDescriptor_Class*> ParentDescriptors;

	TypeDescriptor_Class(void (*InitializationFunction)(TypeDescriptor_Class*)) : TypeDescriptor{ "", 0 } {
		InitializationFunction(this);
	}
	TypeDescriptor_Class(const char* name, size_t size, const std::initializer_list<FProperty>& PropertyList, const std::initializer_list<FFunction>& FunctionList) : TypeDescriptor{ "", 0 }, Properties{ PropertyList }, Functions{ FunctionList }{

	}
	virtual const TString ToString(const void* Instance, const int32 Indent = 0) const override {
		TString result = Name + " {" + '\n';
		for (const FProperty& member : Properties) {
			TString memberValue = member.GetType()->ToString((char*)Instance + member.Offset, Indent + 1);
			result += std::string(4 * ((int64)Indent + 1), ' ') + member.GetName() + TString(" = ") + memberValue;
			result += '\n';
		}
		result += std::string(4 * (int64)Indent, ' ') + "}";
		return result;
	}
};

