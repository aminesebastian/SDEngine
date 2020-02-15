#pragma once
#include "Core/Pictorum/IWidgetSlot.h"
#include "Core/Pictorum/PictorumDataTypes.h"

class LayoutWidgetSlot : public IWidgetSlot {
public:
	LayoutWidgetSlot();

	const FOffsets& GetOffsets() const;
	LayoutWidgetSlot* SetOffset(const EPictorumSide& Side, const float& Offset);
private:
	FOffsets Offsets;
};

