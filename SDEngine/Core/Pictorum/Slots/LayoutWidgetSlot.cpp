#include "LayoutWidgetSlot.h"

LayoutWidgetSlot::LayoutWidgetSlot() {
	bIsRelative = true;
}

void LayoutWidgetSlot::SetTopOffset(const float& TopOffset) {
	Offsets.SetTop(TopOffset);
}
void LayoutWidgetSlot::SetRightOffset(const float& RightOffset) {
	Offsets.SetRight(RightOffset);
}
void LayoutWidgetSlot::SetBottomOffset(const float& BottomOffset) {
	Offsets.SetBottom(BottomOffset);
}
void LayoutWidgetSlot::SetLeftOffset(const float& LeftOffset) {
	Offsets.SetLeft(LeftOffset);
}
void LayoutWidgetSlot::SetOffsets(const float& Top, const float& Right, const float& Bottom, const float& Left) {
	Offsets.SetTop(Top);
	Offsets.SetRight(Right);
	Offsets.SetBottom(Bottom);
	Offsets.SetLeft(Left);
}
const FAnchors& LayoutWidgetSlot::GetOffsets() {
	return Offsets;
}
void LayoutWidgetSlot::SetOffsetRelative(bool Relative) {
	bIsRelative = Relative;
}
const bool& LayoutWidgetSlot::GetOffsetsRelative() const {
	return bIsRelative;
}

void LayoutWidgetSlot::SetTopMargin(const float& TopMargin) {
	Margins.SetTop(TopMargin);
}
void LayoutWidgetSlot::SetRightMargin(const float& RightMargin) {
	Margins.SetRight(RightMargin);
}
void LayoutWidgetSlot::SetBottomMargin(const float& BottomMargin) {
	Margins.SetBottom(BottomMargin);
}
void LayoutWidgetSlot::SetLeftMargin(const float& LeftMargin) {
	Margins.SetLeft(LeftMargin);
}
void LayoutWidgetSlot::SetMargins(const float& Top, const float& Right, const float& Bottom, const float& Left) {
	Margins.SetTop(Top);
	Margins.SetRight(Right);
	Margins.SetBottom(Bottom);
	Margins.SetLeft(Left);
}
const FMargins& LayoutWidgetSlot::GetMargins() {
	return Margins;
}