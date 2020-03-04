#include "InspectorPanelManager.h"
#include "Editor/Subsystems/EntityInspector/Generators/DefaultInspectorPanelGenerator.h"

InspectorPanelManager::InspectorPanelManager() {
	Default = new DefaultInspectorPanelGenerator();
}
InspectorPanelManager::~InspectorPanelManager() {

}
InspectorPanelManager* InspectorPanelManager::Get() {
	static InspectorPanelManager* Instance = new InspectorPanelManager(); // only initialized once!
	return Instance;
}
void InspectorPanelManager::RegisterCustomization(TypeDescriptor* Descriptor, IInspectorPanelGenerator* Customization) {
	Generators.emplace(Descriptor, Customization);
}
IInspectorPanelGenerator* InspectorPanelManager::GetGenerator(const TypeDescriptor* Descriptor) const {
	if (Generators.find(Descriptor) != Generators.end()) {
		return Generators.at(Descriptor);
	}
	return Default;
}