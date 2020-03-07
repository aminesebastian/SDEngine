#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Editor/Subsystems/EntityInspector/InspectorPanelGenerator.h"
#include "Core/Reflection/Reflection.h"

class InspectorPanelManager {
public:
	virtual ~InspectorPanelManager();
	static InspectorPanelManager* Get();

	void RegisterCustomization(TypeDescriptor* Descriptor, IInspectorPanelGenerator* Customization);
	IInspectorPanelGenerator* GetGenerator(const TypeDescriptor* Descriptor) const;
private:
	SHashMap<const TypeDescriptor*, IInspectorPanelGenerator*> Generators;
	IInspectorPanelGenerator* Default;

	InspectorPanelManager();
	IInspectorPanelGenerator* GetClosestMatchingGeneratorForClass(const TypeDescriptor_Class* Type) const;
};

