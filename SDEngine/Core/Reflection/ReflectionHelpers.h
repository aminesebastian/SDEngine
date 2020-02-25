#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Reflection/ReflectableTypes.h"
#include "Core/Reflection/TypeResolver.h"

class ReflectionHelpers {
public:
	static const SArray<TypeDescriptor_Class::Member> GetAllMembersOfClass(const class EngineObject* Object);
private:
	static void worker_GetAllMembersOfClass(const TypeDescriptor_Class* CurrentNode, SArray<TypeDescriptor_Class::Member>& Output);
};

