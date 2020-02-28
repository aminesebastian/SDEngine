#pragma once
#include "Editor/EngineUI/Inspector/InspectorPanelGenerator.h"
#include "Editor/EngineUI/Inspector/InspectorPanelBuilder.h"
#include "Core/Reflection/Reflection.h"

class DefaultInspectorPanelGenerator : public IInspectorPanelGenerator{
public:
	virtual void GenerateInspector(InspectorPanelBuilder& Builder) override;
};

