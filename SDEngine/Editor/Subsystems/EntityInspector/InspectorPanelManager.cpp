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
void InspectorPanelManager::RegisterCustomization(ITypeDescriptor* Descriptor, IInspectorPanelGenerator* Customization) {
	Generators.emplace(Descriptor, Customization);
}
IInspectorPanelGenerator* InspectorPanelManager::GetGenerator(const ITypeDescriptor* Descriptor) const {
	if (Generators.find(Descriptor) != Generators.end()) {
		return Generators.at(Descriptor);
	}

	// Recursively check parents if this is a class.
	if (IsA<const TypeDescriptor_Class>(Descriptor)) {
		const TypeDescriptor_Class* classDescriptor = Cast<const TypeDescriptor_Class>(Descriptor);
		IInspectorPanelGenerator* classGen = GetClosestMatchingGeneratorForClass(classDescriptor);
		if (classGen) {
			return classGen;
		}
	} 

	// If no matches are found, return the default.
	return Default;
}
IInspectorPanelGenerator* InspectorPanelManager::GetClosestMatchingGeneratorForClass(const TypeDescriptor_Class* Type) const {
	if (Generators.find(Type) != Generators.end()) {
		return Generators.at(Type);
	}
	for (const TypeDescriptor_Class* desc : Type->GetParentClasses()) {
		IInspectorPanelGenerator* parentGen = GetClosestMatchingGeneratorForClass(desc);
		if (parentGen) {
			return parentGen;
		}
	}
	return nullptr;
}