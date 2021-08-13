#pragma once
#include "Core/DataTypes/TypeDefenitions.h"

enum class EPrimitiveDataType : uint8 {
	Void,
	Integer,
	Float,
	Vector2D,
	Vector3D,
	Vector4D,
	Bool,
	String,
	Class,
	Struct,
	Enum,
	Array
};

class ITypeDescriptor {
public:
	ITypeDescriptor(const TString& Name, const size_t& Size, const EPrimitiveDataType& Type) : Name(Name), Size(Size), PrimitiveType(Type) {}
	virtual ~ITypeDescriptor() {}
	const TString& GetName() const {
		return Name;
	}
	const size_t& GetSize() const {
		return Size;
	}
	const EPrimitiveDataType& GetPrimitiveType() const {
		return PrimitiveType;
	}
	virtual const TString ToString(const void* Instance, const int32 Indent = 0) const = 0;
protected:
	TString Name;
	size_t Size;
	EPrimitiveDataType PrimitiveType;
};