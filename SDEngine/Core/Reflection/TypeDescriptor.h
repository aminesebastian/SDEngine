#pragma once
#include "Core/DataTypes/TypeDefenitions.h"

struct TypeDescriptor {
	TypeDescriptor(const TString& Name, const size_t& Size) : Name{ Name }, Size{ Size } {}
	virtual ~TypeDescriptor() {}
	virtual const TString ToString(const void* Instance) const = 0;
	TString Name;
	size_t Size;
};