#include "LayoutWidgetSlot.h"

LayoutWidgetSlot::LayoutWidgetSlot() {

}

const FOffsets& LayoutWidgetSlot::GetOffsets() const{ 
	return Offsets;
}
LayoutWidgetSlot* LayoutWidgetSlot::SetOffset(const EPictorumSide& Side, const float& Offset) {
	Offsets.SetSide(Side, Offset);
	return this;
}
