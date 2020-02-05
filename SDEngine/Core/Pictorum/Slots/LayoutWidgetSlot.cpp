#include "LayoutWidgetSlot.h"

LayoutWidgetSlot::LayoutWidgetSlot() {
	bIsRelative = true;
}

void LayoutWidgetSlot::SetTopOffset(const float& TopOffset) {
	Offsets.Top = TopOffset;
}
void LayoutWidgetSlot::SetRightOffset(const float& RightOffset) {
	Offsets.Right = RightOffset;
}
void LayoutWidgetSlot::SetBottomOffset(const float& BottomOffset) {
	Offsets.Bottom = BottomOffset;
}
void LayoutWidgetSlot::SetLeftOffset(const float& LeftOffset) {
	Offsets.Left = LeftOffset;
}
void LayoutWidgetSlot::SetOffsets(const float& Top, const float& Right, const float& Bottom, const float& Left) {
	Offsets.Top = Top;
	Offsets.Right = Right;
	Offsets.Bottom = Bottom;
	Offsets.Left = Left;
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
	Margins.Top = TopMargin;
}
void LayoutWidgetSlot::SetRightMargin(const float& RightMargin) {
	Margins.Right = RightMargin;
}
void LayoutWidgetSlot::SetBottomMargin(const float& BottomMargin) {
	Margins.Bottom = BottomMargin;
}
void LayoutWidgetSlot::SetLeftMargin(const float& LeftMargin) {
	Margins.Left = LeftMargin;
}
void LayoutWidgetSlot::SetMargins(const float& Top, const float& Right, const float& Bottom, const float& Left) {
	Margins.Top = Top;
	Margins.Right = Right;
	Margins.Bottom = Bottom;
	Margins.Left = Left;
}
const FMargins& LayoutWidgetSlot::GetMargins() {
	return Margins;
}