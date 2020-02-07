#pragma once
#include "Core/Pictorum/PictorumDataTypes.h"

class IWidgetSlot {
public:
	IWidgetSlot() = default;
	virtual ~IWidgetSlot() = default; 
	virtual void ModifyGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry);
};

