#include "LayoutSlot.h"

LayoutWidgetSlot::LayoutWidgetSlot() {
	HorizontalFillRule = EFillRule::AUTOMATIC;
	VerticalFillRule   = EFillRule::AUTOMATIC;
}

const FOffsets& LayoutWidgetSlot::GetOffsets() const{ 
	return Offsets;
}
LayoutWidgetSlot* LayoutWidgetSlot::SetOffset(const EPictorumSide& Side, const float& Offset) {
	Offsets.SetSide(Side, Offset);
	return this;
}

const EFillRule& LayoutWidgetSlot::GetHorizontalFillRule() const {
	return HorizontalFillRule;
}
LayoutWidgetSlot* LayoutWidgetSlot::SetHorizontalFillRule(const EFillRule& Rule) {
	HorizontalFillRule = Rule;
	return this;
}
const EFillRule& LayoutWidgetSlot::GetVerticalFillRule() const {
	return VerticalFillRule;
}
LayoutWidgetSlot* LayoutWidgetSlot::SetVerticalFillRule(const EFillRule& Rule) {
	VerticalFillRule = Rule;
	return this;
}