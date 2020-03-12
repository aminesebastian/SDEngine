#pragma once
#include "Core/DataStructures/DataStructures.h"
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Reflection/Utilities/TypeDescriptor.h"

class TypeDescriptor_Vector4D : public ITypeDescriptor {
public:
	TypeDescriptor_Vector4D() : ITypeDescriptor("Vector4D", sizeof(Vector4D), EPrimitiveDataType::Vector4D) {}
	virtual const TString ToString(const void* Instance, const int32 Indent = 0) const override {
		Vector4D vec = *(const Vector4D*)Instance;
		return Name + "{x:" + std::to_string(vec.x) + " y:" + std::to_string(vec.y) + " z:" + std::to_string(vec.z) + " w:" + std::to_string(vec.w) + "}";
	}
};
class TypeDescriptor_Vector3D : public ITypeDescriptor {
public:
	TypeDescriptor_Vector3D() : ITypeDescriptor("Vector3D", sizeof(Vector3D), EPrimitiveDataType::Vector3D) {}
	virtual const TString ToString(const void* Instance, const int32 Indent = 0) const override {
		Vector3D vec = *(const Vector3D*)Instance;
		return Name + "{x:" + std::to_string(vec.x) + " y:" + std::to_string(vec.y) + " z:" + std::to_string(vec.z) + "}";
	}
};
class TypeDescriptor_Vector2D : public ITypeDescriptor {
public:
	TypeDescriptor_Vector2D() : ITypeDescriptor("Vector2D", sizeof(Vector2D), EPrimitiveDataType::Vector2D) {}
	virtual const TString ToString(const void* Instance, const int32 Indent = 0) const override {
		Vector2D vec = *(const Vector2D*)Instance;
		return Name + "{x:" + std::to_string(vec.x) + " y:" + std::to_string(vec.y) + "}";
	}
};

class TypeDescriptor_Float : public ITypeDescriptor {
public:
	TypeDescriptor_Float() : ITypeDescriptor("float", sizeof(float), EPrimitiveDataType::Float) {}
	virtual const TString ToString(const void* Instance, const int32 Indent = 0) const override {
		return Name + "{" + std::to_string(*(const float*)Instance) + "}";
	}
};

class TypeDescriptor_Int32 : public ITypeDescriptor {
public:
	TypeDescriptor_Int32() : ITypeDescriptor("int32", sizeof(int32), EPrimitiveDataType::Integer) {}
	virtual const TString ToString(const void* Instance, const int32 Indent = 0) const override {
		return Name + "{" + std::to_string(*(const int32*)Instance) + "}";
	}
};

class TypeDescriptor_Bool : public ITypeDescriptor {
public:
	TypeDescriptor_Bool() : ITypeDescriptor("bool", sizeof(bool), EPrimitiveDataType::Bool) {}
	virtual const TString ToString(const void* Instance, const int32 Indent = 0) const override {
		return Name + "{" + (*(const bool*)Instance ? "true" : "false") + "}";
	}
};

class TypeDescriptor_Void : public ITypeDescriptor {
public:
	TypeDescriptor_Void() : ITypeDescriptor("void", 0, EPrimitiveDataType::Void) {}
	virtual const TString ToString(const void* Instance, const int32 Indent = 0) const override {
		return Name + "{VOID}";
	}
};

class TypeDescriptor_TString : public ITypeDescriptor {
public:
	TypeDescriptor_TString() : ITypeDescriptor("String", sizeof(TString), EPrimitiveDataType::String) {}
	virtual const TString ToString(const void* Instance, const int32 Indent = 0) const override {
		return Name + "{" + *(const TString*)Instance + "}";
	}
};
template<typename T>
class TypeDescriptor_SArray : public ITypeDescriptor {
public:
	TypeDescriptor_SArray() : ITypeDescriptor("Array", sizeof(SArray<T>), EPrimitiveDataType::Array) {
		ContainerType = TypeResolver<T>::Get();
	}
	virtual const TString ToString(const void* Instance, const int32 Indent = 0) const override {
		return Name + "{}";
	}
	ITypeDescriptor* ContainerType;
};