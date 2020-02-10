#pragma once
#include "Core/Pictorum/IWidgetSlot.h"
#include "Core/Pictorum/PictorumDataTypes.h"

class LayoutWidgetSlot : public IWidgetSlot {
public:
	LayoutWidgetSlot();

	const FAnchors& GetOffsets() const;
	LayoutWidgetSlot* SetOffset(const EPictorumSide& Side, const float& Offset);
	LayoutWidgetSlot* SetOffsetRelative(const EPictorumSide& Side);
	LayoutWidgetSlot* SetOffsetAbsolute(const EPictorumSide& Side);

	const FMargins& GetMargins() const;
	LayoutWidgetSlot* SetMargin(const EPictorumSide& Side, const float& Margin);
private:
	FAnchors Offsets;
	FMargins Margins;
};

