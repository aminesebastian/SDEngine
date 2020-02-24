#pragma once
#include "Core/Reflection/TypeDescriptor.h"

struct TypeDescriptor_Float : public TypeDescriptor {
	TypeDescriptor_Float() : TypeDescriptor{ "float", sizeof(float) } {}
	virtual const TString ToString(const void* Instance) const override {
		return Name + "{" + std::to_string(*(const double*)Instance) + "}";
	}
};

struct TypeDescriptor_Int32 : public TypeDescriptor {
	TypeDescriptor_Int32() : TypeDescriptor{ "int32", sizeof(int32) } {}
	virtual const TString ToString(const void* Instance) const override {
		return Name + "{" + std::to_string(*(const int32*)Instance) + "}";
	}
};

struct TypeDescriptor_TString : public TypeDescriptor {
	TypeDescriptor_TString() : TypeDescriptor{ "TString", sizeof(double) } {}
	virtual const TString ToString(const void* Instance) const override {
		return Name + "{" + *(const TString*)Instance + "}";
	}
};

struct TypeDescriptor_Struct : public TypeDescriptor {
	struct Member {
		const char* Name;
		size_t Offset;
		TypeDescriptor* Type;
	};

	std::vector<Member> Members;

	TypeDescriptor_Struct(void (*InitializationFunction)(TypeDescriptor_Struct*)) : TypeDescriptor{ "", 0 } {
		InitializationFunction(this);
	}
	TypeDescriptor_Struct(const char* name, size_t size, const std::initializer_list<Member>& InitializationList) : TypeDescriptor{ "", 0 }, Members{ InitializationList } {
	}
	virtual const TString ToString(const void* Instance) const override {
		return "";
	}
};