#include "LayoutSlot.h"

LayoutWidgetSlot::LayoutWidgetSlot() {

}

const FOffsets& LayoutWidgetSlot::GetOffsets() const{ 
	return Offsets;
}
LayoutWidgetSlot* LayoutWidgetSlot::SetOffset(const EPictorumSide& Side, const float& Offset) {
	Offsets.SetSide(Side, Offset);
	return this;
}

const FPivotOffset& LayoutWidgetSlot::GetPivotOffset() const {
	return PivotOffset;
}
LayoutWidgetSlot* LayoutWidgetSlot::SetPivotOffset(const float& X, const float& Y) {
	PivotOffset.SetXOffset(X);
	PivotOffset.SetYOffset(Y);
	return this;
}

const FAnchors& LayoutWidgetSlot::GetAnchors() const {
	return Anchors;
}
LayoutWidgetSlot* LayoutWidgetSlot::SetAnchor(const EPictorumSide& Side, const float& AnchorPosition) {
	Anchors.SetSide(Side, AnchorPosition);
	return this;
}