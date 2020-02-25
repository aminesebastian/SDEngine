#include "ReflectionHelpers.h"
#include "Core/Objects/EngineObject.h"

const SArray<TypeDescriptor_Class::Member> ReflectionHelpers::GetAllMembersOfClass(const EngineObject* Object) {
	SArray<TypeDescriptor_Class::Member> output;
	worker_GetAllMembersOfClass(Object->StaticDescriptor(), output);
	return output;
}
void ReflectionHelpers::worker_GetAllMembersOfClass(const TypeDescriptor_Class* CurrentNode, SArray<TypeDescriptor_Class::Member>& Output) {
	// Add the members of the current node.
	for (const TypeDescriptor_Class::Member& member : CurrentNode->Members) {
		Output.Add(member);
	}

	// Continue down the tree.
	for (TypeDescriptor* desc : CurrentNode->ParentDescriptors) {
		TypeDescriptor_Class* parent = (TypeDescriptor_Class*)desc;
		if (parent) {
			worker_GetAllMembersOfClass((TypeDescriptor_Class*)desc, Output);
		}
	}
}