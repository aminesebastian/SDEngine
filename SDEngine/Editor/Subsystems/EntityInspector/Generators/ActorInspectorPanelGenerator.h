#pragma once
#include "Editor/Subsystems/EntityInspector/InspectorPanelGenerator.h"
#include "Editor/Subsystems/EntityInspector/InspectorPanelBuilder.h"
#include "Core/Reflection/Reflection.h"

class ActorInspectorPanelGenerator : public IInspectorPanelGenerator{
public:
	virtual void GenerateInspector(InspectorPanelBuilder& Builder) override;
};

