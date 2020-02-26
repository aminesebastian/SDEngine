#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Reflection/ReflectableTypes.h"
#include "Core/Reflection/TypeResolver.h"

class ReflectionHelpers {
public:
	static const SArray<TypeDescriptor_Class::Member> GetAllMembersOfClass(const class EngineObject* Object);

	//template<typename T>
	//static const void* GetStartingLocationForParentClass(void* Object) {
	//	return reinterpret_cast<char*>(static_cast<T*>(Object)) - reinterpret_cast<char*>(Object);
	//}
private:
	static void worker_GetAllMembersOfClass(const TypeDescriptor_Class* CurrentNode, SArray<TypeDescriptor_Class::Member>& Output, const size_t& PreviousSize);
};

