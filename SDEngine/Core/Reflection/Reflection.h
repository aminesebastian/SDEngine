#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Reflection/Runtime/TypeResolver.h"
#include "Core/Reflection/Types/ComplexReflectableTypes.h"
#include "Core/Reflection/Types/ReflectableTypes.h"
#include "Core/Reflection/Utilities/ReflectionDataTypes.h"

template <typename T>
ITypeDescriptor* GetPrimitiveDescriptor();

#define SD_STRUCT()
#define SD_CLASS()
#define SD_PROPERTY(...) 
#define SD_FUNCTION(...) 

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
    TypeDescriptor_Struct Type::Reflection{#Type, sizeof(Type), Type::InitializeReflection}; \
    void Type::InitializeReflection(TypeDescriptor_Struct* TypeDescriptorIn) { \
        using T = Type; \

#define REFLECT_STRUCT_PARENT(Parent)

#define REFLECT_STRUCT_MEMBER(Name, InspectorName, Category, InspectorHidden) \
		FPropertyMetadata metadata##Name(Category, InspectorHidden); \
        TypeDescriptorIn->AddProperty(new FProperty(#Name, InspectorName, offsetof(T, Name), TypeResolver<decltype(T::Name)>::Get(), metadata##Name)); \

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
    TypeDescriptor_Class Type::Reflection{#Type, sizeof(Type), Type::InitializeReflection}; \
    void Type::InitializeReflection(TypeDescriptor_Class* TypeDescriptorIn) { \
        using T = Type; \

#define REFLECT_CLASS_PARENT(Parent) \
		TypeDescriptorIn->AddParent((TypeDescriptor_Class*)TypeResolver<Parent>::Get());

#define REFLECT_CLASS_MEMBER(Name, InspectorName, Category, InspectorHidden) \
		FPropertyMetadata metadata##Name(Category, InspectorHidden); \
        TypeDescriptorIn->AddProperty(new FProperty(#Name, InspectorName, offsetof(T, Name), TypeResolver<decltype(T::Name)>::Get(), metadata##Name)); \

#define REFLECT_CLASS_FUNCTION(MethodName, ConstReturn, ReturnType, ReturnMemoryType, ConstFunction, ...) \
		TypeDescriptorIn->AddFunction(new FFunction(#MethodName, ConstReturn, TypeResolver<ReturnType>::Get(), static_cast<ETypeMemoryType>(ReturnMemoryType), ConstFunction, { })); \

#define REFLECT_CLASS_END() \
    }

