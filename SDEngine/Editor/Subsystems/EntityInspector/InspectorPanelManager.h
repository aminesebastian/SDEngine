#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Editor/Subsystems/EntityInspector/InspectorPanelGenerator.h"
#include "Core/Reflection/Reflection.h"

class InspectorPanelManager {
public:
	virtual ~InspectorPanelManager();
	static InspectorPanelManager* Get();

	void RegisterCustomization(ITypeDescriptor* Descriptor, IInspectorPanelGenerator* Customization);
	IInspectorPanelGenerator* GetGenerator(const ITypeDescriptor* Descriptor) const;
private:
	SHashMap<const ITypeDescriptor*, IInspectorPanelGenerator*> Generators;
	IInspectorPanelGenerator* Default;

	InspectorPanelManager();
	IInspectorPanelGenerator* GetClosestMatchingGeneratorForClass(const TypeDescriptor_Class* Type) const;
};

