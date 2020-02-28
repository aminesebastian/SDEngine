#include "DefaultInspectorPanelGenerator.h"
#include "Core/Reflection/ReflectionHelpers.h"

void DefaultInspectorPanelGenerator::GenerateInspector(InspectorPanelBuilder& Builder) {
	const EngineObject* target = Builder.GetTarget<EngineObject>();
	SArray<FProperty> properties;
	ReflectionHelpers::GetAllMembersOfClass(properties, target);

	for (const FProperty& prop : properties) {
		if (prop.bInspectorHidden) {
			continue;
		}
		Builder.AddControlForProperty(prop);
	}
}