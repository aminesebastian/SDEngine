#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Reflection/ReflectableTypes.h"
#include "Core/Reflection/TypeResolver.h"

template <typename T>
TypeDescriptor* GetPrimitiveDescriptor();

#define SD_STRUCT_PROPERTIES() \
    friend struct DefaultResolver; \
    static TypeDescriptor_Struct Reflection; \
    static void InitializeReflection(TypeDescriptor_Struct*);

#define REFLECT_STRUCT_BEGIN(Type) \
    TypeDescriptor_Struct Type::Reflection{Type::InitializeReflection}; \
    void Type::InitializeReflection(TypeDescriptor_Struct* TypeDescriptorIn) { \
        using T = Type; \
        TypeDescriptorIn->Name = #Type; \
        TypeDescriptorIn->Size = sizeof(T); \
        TypeDescriptorIn->Members = {

#define REFLECT_STRUCT_MEMBER(Name) \
            {#Name, offsetof(T, Name), TypeResolver<decltype(T::Name)>::Get()},

#define REFLECT_STRUCT_END() \
        }; \
    }

#define SD_STRUCT()
#define SD_CLASS()
#define SD_PROPERTY(...)