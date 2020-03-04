#include "DefaultInspectorPanelGenerator.h"
#include "Core/Reflection/ReflectionHelpers.h"

void DefaultInspectorPanelGenerator::GenerateInspector(InspectorPanelBuilder& Builder) {
	if (IsA<const TypeDescriptor_Class>(Builder.GetTypeDescriptor())) {
		const EngineObject* target = Builder.GetTarget<EngineObject>();
		SArray<FProperty> properties;
		ReflectionHelpers::GetAllMembersOfClass(properties, target);

		for (const FProperty& prop : properties) {
			if (prop.bInspectorHidden) {
				continue;
			}
			Builder.AddControlForProperty(prop);
		}
	} else if (IsA<const TypeDescriptor_Struct>(Builder.GetTypeDescriptor())) {
		const TypeDescriptor_Struct* typeDesc = Cast<const TypeDescriptor_Struct>(Builder.GetTypeDescriptor());
		for (const FProperty& prop : typeDesc->Properties) {
			if (prop.bInspectorHidden) {
				continue;
			}
			Builder.AddControlForProperty(prop);
		}
	}
}