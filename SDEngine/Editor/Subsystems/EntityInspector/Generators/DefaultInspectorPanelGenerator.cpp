#include "DefaultInspectorPanelGenerator.h"
#include "Core/Reflection/ReflectionHelpers.h"

void DefaultInspectorPanelGenerator::GenerateInspector(InspectorPanelBuilder& Builder) {
	if (IsA<const TypeDescriptor_Class>(Builder.GetTypeDescriptor())) {
		const EngineObject* target = Builder.GetTarget<EngineObject>();
		SArray<FProperty*> properties;
		ReflectionHelpers::GetAllMembersOfClass(properties, target);

		for (FProperty* prop : properties) {
			if (prop->GetMetadata().GetHiddenInInspector()) {
				continue;
			}
			Builder.AddControlForProperty(prop);
		}
	} else if (IsA<const TypeDescriptor_Struct>(Builder.GetTypeDescriptor())) {
		TypeDescriptor* typeDesc = RemoveConst(Builder.GetTypeDescriptor());
		TypeDescriptor_Struct* classTypeDesc = Cast<TypeDescriptor_Struct>(typeDesc);
		for (FProperty& prop : classTypeDesc->Properties) {
			if (prop.GetMetadata().GetHiddenInInspector()) {
				continue;
			}
			Builder.AddControlForProperty(&prop);
		}
	}
}