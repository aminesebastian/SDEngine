#pragma once
#include "Core/Pictorum/IWidgetSlot.h"
#include "Core/Pictorum/PictorumDataTypes.h"

class PictorumCanvasSlot : public IWidgetSlot {
public:
	PictorumCanvasSlot();

	FOffsets Offsets;
	FAnchors Anchors;

};

