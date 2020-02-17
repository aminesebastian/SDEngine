#pragma once
#include "Core/Pictorum/IWidgetSlot.h"
#include "Core/Pictorum/PictorumDataTypes.h"

class LayoutWidgetSlot : public IWidgetSlot {
public:
	LayoutWidgetSlot();

	const FOffsets& GetOffsets() const;
	LayoutWidgetSlot* SetOffset(const EPictorumSide& Side, const float& Offset);

	const FPivotOffset& GetPivotOffset() const;
	LayoutWidgetSlot* SetPivotOffset(const float& X, const float& Y);

	const FAnchors& GetAnchors() const;
	LayoutWidgetSlot* SetAnchor(const EPictorumSide& Side, const float& AnchorPosition);

private:
	FOffsets Offsets;
	FAnchors Anchors;
	FPivotOffset PivotOffset;
};

