#include "ActorInspectorPanelGenerator.h"
#include "Core/Objects/Entities/Actor.h"
#include "Core/Reflection/ReflectionHelpers.h"

void ActorInspectorPanelGenerator::GenerateInspector(InspectorPanelBuilder& Builder) {
	const Actor* target = Builder.GetTarget<Actor>();

	Builder.AddControlForProperty(ReflectionHelpers::GetPropertyHandle("Transform", target));

	SArray<FProperty*> properties;
	ReflectionHelpers::GetAllMembersOfClass(properties, target);

	for (const FProperty* prop : properties) {
		if (prop->bInspectorHidden) {
			continue;
		}
		Builder.AddControlForProperty(prop);
	}
}