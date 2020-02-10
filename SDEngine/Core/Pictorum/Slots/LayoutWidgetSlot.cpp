#include "LayoutWidgetSlot.h"

LayoutWidgetSlot::LayoutWidgetSlot() {

}

const FAnchors& LayoutWidgetSlot::GetOffsets() const{ 
	return Offsets;
}
LayoutWidgetSlot* LayoutWidgetSlot::SetOffset(const EPictorumSide& Side, const float& Offset) {
	Offsets.SetSide(Side, Offset);
	return this;
}
LayoutWidgetSlot* LayoutWidgetSlot::SetOffsetRelative(const EPictorumSide& Side) {
	Offsets.SetRelative(Side);
	return this;
}
LayoutWidgetSlot* LayoutWidgetSlot::SetOffsetAbsolute(const EPictorumSide& Side) {
	Offsets.SetAbsolute(Side);
	return this;
}

const FMargins& LayoutWidgetSlot::GetMargins() const{ 
	return Margins;
}
LayoutWidgetSlot* LayoutWidgetSlot::SetMargin(const EPictorumSide& Side, const float& Margin) {
	Margins.SetSide(Side, Margin);
	return this;
}