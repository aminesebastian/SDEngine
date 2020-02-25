#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Reflection/ReflectableTypes.h"
#include "Core/Reflection/TypeResolver.h"

template <typename T>
TypeDescriptor* GetPrimitiveDescriptor();

#define SD_STRUCT()
#define SD_CLASS()
#define SD_PROPERTY(...)

#define SD_STRUCT_BODY() \
    friend struct DefaultResolver; \
    static TypeDescriptor_Struct Reflection; \
    static void InitializeReflection(TypeDescriptor_Struct*); \

#define REFLECT_STRUCT_BEGIN(Type) \
    TypeDescriptor_Struct Type::Reflection{Type::InitializeReflection}; \
    void Type::InitializeReflection(TypeDescriptor_Struct* TypeDescriptorIn) { \
        using T = Type; \
        TypeDescriptorIn->Name = #Type; \
        TypeDescriptorIn->Size = sizeof(T); \

#define REFLECT_STRUCT_BEGIN_PARENTS() 
#define REFLECT_STRUCT_PARENT(Parent)
#define REFLECT_STRUCT_PARENT_END()

#define REFLECT_STRUCT_MEMBERS_BEGIN() \
        TypeDescriptorIn->Members = { \

#define REFLECT_STRUCT_MEMBER(Name) \
            {#Name, offsetof(T, Name), TypeResolver<decltype(T::Name)>::Get()}, \

#define REFLECT_STRUCT_MEMBERS_END() \
	    }; \

#define REFLECT_STRUCT_END() \
    }


#define SD_CLASS_BODY() \
    friend struct DefaultResolver; \
    static TypeDescriptor_Class Reflection; \
    static void InitializeReflection(TypeDescriptor_Class*); \
	public: \
	virtual const TypeDescriptor_Class* StaticDescriptor() const override; \
	private: \

#define REFLECT_CLASS_BEGIN(Type) \
	const TypeDescriptor_Class* Type::StaticDescriptor() const {\
		return &Type::Reflection; \
	}\
    TypeDescriptor_Class Type::Reflection{Type::InitializeReflection}; \
    void Type::InitializeReflection(TypeDescriptor_Class* TypeDescriptorIn) { \
        using T = Type; \
        TypeDescriptorIn->Name = #Type; \
        TypeDescriptorIn->Size = sizeof(T); 

#define REFLECT_CLASS_BEGIN_PARENTS() \
		TypeDescriptorIn->ParentDescriptors = { 

#define REFLECT_CLASS_PARENT(Parent) \
			{TypeResolver<Parent>::Get()},

#define REFLECT_CLASS_PARENT_END() \
        }; \

#define REFLECT_CLASS_MEMBERS_BEGIN() \
        TypeDescriptorIn->Members = { \

#define REFLECT_CLASS_MEMBER(Name) \
            {#Name, offsetof(T, Name), TypeResolver<decltype(T::Name)>::Get()}, \

#define REFLECT_CLASS_MEMBERS_END() \
	    }; \

#define REFLECT_CLASS_END() \
    }

