#include "Core/Reflection/Utilities/ReflectionHelpers.h"
#include "DefaultInspectorPanelGenerator.h"

void DefaultInspectorPanelGenerator::GenerateInspector(InspectorPanelBuilder& Builder) {
	ReflectionHandle handle = Builder.GetTarget();

	for (PropertyHandle prop : handle) {
		if (!prop.GetMetadata().GetHiddenInInspector()) {
			Builder.AddControlForProperty(prop);
		}
	}
}