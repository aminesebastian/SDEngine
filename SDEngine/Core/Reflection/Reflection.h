#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Reflection/ReflectableTypes.h"
#include "Core/Reflection/TypeResolver.h"

template <typename T>
TypeDescriptor* GetPrimitiveDescriptor();

#define OverrideInspectorName = 0;
#define InspectorHidden = 0;
#define Category = 0;

#define SD_STRUCT()
#define SD_CLASS()
#define SD_PROPERTY(...)

#define SD_STRUCT_BODY() \
	private: \
    friend struct DefaultResolver; \
    static TypeDescriptor_Struct Reflection; \
    static void InitializeReflection(TypeDescriptor_Struct*); \
	public: \
	const TypeDescriptor_Struct* StaticDescriptor() const; \

#define REFLECT_STRUCT_BEGIN(Type) \
	const TypeDescriptor_Struct* Type::StaticDescriptor() const {\
		return &Type::Reflection; \
	}\
    TypeDescriptor_Struct Type::Reflection{Type::InitializeReflection}; \
    void Type::InitializeReflection(TypeDescriptor_Struct* TypeDescriptorIn) { \
        using T = Type; \
        TypeDescriptorIn->Name = #Type; \
        TypeDescriptorIn->Size = sizeof(T); \

#define REFLECT_STRUCT_BEGIN_PARENTS() 
#define REFLECT_STRUCT_PARENT(Parent)
#define REFLECT_STRUCT_PARENT_END()

#define REFLECT_STRUCT_MEMBERS_BEGIN() \
        TypeDescriptorIn->Properties = { \

#define REFLECT_STRUCT_MEMBER(Name, InspectorName, InspectorHiddenIn, Category) \
            {#Name, InspectorName, Category, offsetof(T, Name), TypeResolver<decltype(T::Name)>::Get(), InspectorHiddenIn}, \

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
        TypeDescriptorIn->Properties = { \

#define REFLECT_CLASS_MEMBER(Name, InspectorName, InspectorHiddenIn, Category) \
            {#Name, InspectorName, Category, offsetof(T, Name), TypeResolver<decltype(T::Name)>::Get(), InspectorHiddenIn}, \

#define REFLECT_CLASS_MEMBERS_END() \
	    }; \

#define REFLECT_CLASS_END() \
    }

