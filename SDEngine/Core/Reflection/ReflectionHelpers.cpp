#include "ReflectionHelpers.h"
#include "Core/Objects/EngineObject.h"

const SArray<TypeDescriptor_Class::Member> ReflectionHelpers::GetAllMembersOfClass(const EngineObject* Object) {
	SArray<TypeDescriptor_Class::Member> output;
	worker_GetAllMembersOfClass(Object->StaticDescriptor(), output, 0);
	return output;
}
void ReflectionHelpers::worker_GetAllMembersOfClass(const TypeDescriptor_Class* CurrentNode, SArray<TypeDescriptor_Class::Member>& Output, const size_t& PreviousSize) {
	// Add the members of the current node.
	for (TypeDescriptor_Class::Member member : CurrentNode->Members) {
		member.Offset += 0;
		Output.Add(member);
	}

	// Continue down the tree.
	for (TypeDescriptor* desc : CurrentNode->ParentDescriptors) {
		TypeDescriptor_Class* parent = (TypeDescriptor_Class*)desc;
		if (parent) {
			worker_GetAllMembersOfClass((TypeDescriptor_Class*)desc, Output, PreviousSize + parent->Size);
		}
	}
}