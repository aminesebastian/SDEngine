#pragma once
#include "Core/Pictorum/IWidgetSlot.h"
#include "Core/Pictorum/PictorumDataTypes.h"

class LayoutWidgetSlot : public IWidgetSlot {
public:
	LayoutWidgetSlot();

	const FOffsets& GetOffsets() const;
	LayoutWidgetSlot* SetOffset(const EPictorumSide& Side, const float& Offset);

	const EFillRule& GetHorizontalFillRule() const;
	LayoutWidgetSlot* SetHorizontalFillRule(const EFillRule& Rule);
	const EFillRule& GetVerticalFillRule() const;
	LayoutWidgetSlot* SetVerticalFillRule(const EFillRule& Rule);
private:
	FOffsets Offsets;
	EFillRule VerticalFillRule;
	EFillRule HorizontalFillRule;
};

