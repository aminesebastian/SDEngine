#pragma once
#include "Core/Objects/EngineObject.h"
#include "Core/DataStructures/Array.h"
#include "Editor/EngineUI/Inspector/InspectorPanelBuilder.h"

class IInspectorPanelGenerator {
public:
	virtual void GenerateInspector(InspectorPanelBuilder& Builder) = 0;
};