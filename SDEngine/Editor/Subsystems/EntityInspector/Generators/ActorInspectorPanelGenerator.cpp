#include "ActorInspectorPanelGenerator.h"
#include "Core/Objects/Entities/Actor.h"
#include "Core/Reflection/Utilities/ReflectionHelpers.h"

void ActorInspectorPanelGenerator::GenerateInspector(InspectorPanelBuilder& Builder) {
	ReflectionHandle handle = Builder.GetTarget();

	// Ensure transform appears at the top.
	Builder.AddControlForProperty(handle.GetPropertyHandle("Transform"));

	for (PropertyHandle prop : handle) {
		if (!prop.GetMetadata().GetHiddenInInspector()) {
			Builder.AddControlForProperty(prop);
		}
	}
}