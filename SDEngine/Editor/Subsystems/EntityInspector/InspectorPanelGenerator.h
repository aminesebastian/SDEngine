#pragma once
#include "Core/Objects/EngineObject.h"
#include "Core/DataStructures/Array.h"
#include "Editor/Subsystems/EntityInspector/InspectorPanelBuilder.h"

class IInspectorPanelGenerator {
public:
	virtual void GenerateInspector(InspectorPanelBuilder& Builder) = 0;
};