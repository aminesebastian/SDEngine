#pragma once
#include "Core/Reflection/TypeDescriptor.h"
#include "Core/DataStructures/DataStructures.h"
#include "Core/DataTypes/TypeDefenitions.h"
#include <iostream>

struct TypeDescriptor_Vector4D: public TypeDescriptor {
	TypeDescriptor_Vector4D() : TypeDescriptor{ "Vector4D", sizeof(Vector4D) } {}
	virtual const TString ToString(const void* Instance, const int32 Indent = 0) const override {
		Vector4D vec = *(const Vector4D*)Instance;
		return Name + "{x:" + std::to_string(vec.x) + " y:" + std::to_string(vec.y) + " z:" + std::to_string(vec.z) + " w:" + std::to_string(vec.w) + "}";
	}
};
struct TypeDescriptor_Vector3D : public TypeDescriptor {
	TypeDescriptor_Vector3D() : TypeDescriptor{ "Vector3D", sizeof(Vector3D) } {}
	virtual const TString ToString(const void* Instance, const int32 Indent = 0) const override {
		Vector3D vec = *(const Vector3D*)Instance;
		return Name + "{x:" + std::to_string(vec.x) + " y:" + std::to_string(vec.y) + " z:" + std::to_string(vec.z) + "}";
	}
};
struct TypeDescriptor_Vector2D : public TypeDescriptor {
	TypeDescriptor_Vector2D() : TypeDescriptor{ "Vector2D", sizeof(Vector2D) } {}
	virtual const TString ToString(const void* Instance, const int32 Indent = 0) const override {
		Vector2D vec = *(const Vector2D*)Instance;
		return Name + "{x:" + std::to_string(vec.x) + " y:" + std::to_string(vec.y) + "}";
	}
};

struct TypeDescriptor_Float : public TypeDescriptor {
	TypeDescriptor_Float() : TypeDescriptor{ "float", sizeof(float) } {}
	virtual const TString ToString(const void* Instance, const int32 Indent = 0) const override {
		return Name + "{" + std::to_string(*(const float*)Instance) + "}";
	}
};

struct TypeDescriptor_Int32 : public TypeDescriptor {
	TypeDescriptor_Int32() : TypeDescriptor{ "int32", sizeof(int32) } {}
	virtual const TString ToString(const void* Instance, const int32 Indent = 0) const override {
		return Name + "{" + std::to_string(*(const int32*)Instance) + "}";
	}
};

struct TypeDescriptor_Bool : public TypeDescriptor {
	TypeDescriptor_Bool() : TypeDescriptor{ "bool", sizeof(bool) } {}
	virtual const TString ToString(const void* Instance, const int32 Indent = 0) const override {
		return Name + "{" + (*(const bool*)Instance ? "true" : "false") + "}";
	}
};

struct TypeDescriptor_TString : public TypeDescriptor {
	TypeDescriptor_TString() : TypeDescriptor{ "String", sizeof(TString) } {}
	virtual const TString ToString(const void* Instance, const int32 Indent = 0) const override {
		return Name + "{" + *(const TString*)Instance + "}";
	}
};
template<typename T>
struct TypeDescriptor_SArray : public TypeDescriptor {
	TypeDescriptor_SArray() : TypeDescriptor{ "Array", sizeof(SArray<T>) } {
		ContainerType = TypeResolver<T>::Get();
	}
	virtual const TString ToString(const void* Instance, const int32 Indent = 0) const override {
		return Name + "{ }";
	}
	TypeDescriptor* ContainerType;
};

struct FProperty {
	const char* Name;
	const char* InspectorName;
	size_t Offset;
	TypeDescriptor* Type;
	bool bInspectorHidden;
};

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
			TString memberValue = member.Type->ToString((char*)Instance + member.Offset, Indent + 1);
			result += std::string(4 * ((int64)Indent + 1), ' ') + member.Name + TString(" = ") + memberValue;
			result += '\n';
		}
		result += std::string(4 * (int64)Indent, ' ') + "}";
		return result;
	}
};

struct TypeDescriptor_Class : public TypeDescriptor {
	std::vector<FProperty> Properties;
	std::vector<TypeDescriptor*> ParentDescriptors;

	TypeDescriptor_Class(void (*InitializationFunction)(TypeDescriptor_Class*)) : TypeDescriptor{ "", 0 } {
		InitializationFunction(this);
	}
	TypeDescriptor_Class(const char* name, size_t size, const std::initializer_list<FProperty>& InitializationList) : TypeDescriptor{ "", 0 }, Properties{ InitializationList } {

	}
	virtual const TString ToString(const void* Instance, const int32 Indent = 0) const override {
		TString result = Name + " {" + '\n';
		for (const FProperty& member : Properties) {
			TString memberValue = member.Type->ToString((char*)Instance + member.Offset, Indent + 1);
			result += std::string(4 * ((int64)Indent + 1), ' ') + member.Name + TString(" = ") + memberValue;
			result += '\n';
		}
		result += std::string(4 * (int64)Indent, ' ') + "}";
		return result;
	}
};
