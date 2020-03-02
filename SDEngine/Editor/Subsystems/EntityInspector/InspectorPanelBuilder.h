#pragma once
#include "Core/Objects/EngineObject.h"
#include "Core/Pictorum/Containers/PictorumVerticalBox.h"
#include "Core/Reflection/Reflection.h"

class PictorumWidget;

class InspectorPanelBuilder {
public:
	InspectorPanelBuilder(PictorumVerticalBox* Owner, EngineObject* CustomizationTarget);

	template<typename T>
	const T* GetTarget() const {
		return Cast<T>(Target);
	}

	void AddControlForProperty(const FProperty& Property);

private:
	PictorumVerticalBox* Parent;
	EngineObject* Target;
	int32 AddedPropertyCount;
};
