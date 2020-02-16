#include "VerticalBoxSlot.h"

VerticalBoxSlot::VerticalBoxSlot() {
	VerticalAlignment   = EVerticalAlignment::TOP;
	HorizontalAlignment = EHorizontalAlignment::LEFT;
}
VerticalBoxSlot::~VerticalBoxSlot() {

}
const EFillRule& VerticalBoxSlot::GetFillRule() const{ 
	return FillRule.GetFillRule();
}
const float& VerticalBoxSlot::GetFillRatio() const {
	return FillRule.GetFillRatio();
}
const EVerticalAlignment& VerticalBoxSlot::GetVerticalAlignment() const { 
	return VerticalAlignment;
}
const EHorizontalAlignment& VerticalBoxSlot::GetHorizontalAlignment() const{ 
	return HorizontalAlignment;
}
FPadding& VerticalBoxSlot::GetPadding() {
	return Padding;
}
VerticalBoxSlot& VerticalBoxSlot::SetUseWidgetDesiredSpace() {
	FillRule.SetUseWidgetDesiredSpace();
	return *this;
}
VerticalBoxSlot& VerticalBoxSlot::SetFillAvilableSpace(const float& Ratio) {
	FillRule.SetFillAllSpace(Ratio);
	return *this;
}
VerticalBoxSlot& VerticalBoxSlot::SetHorizontalAlignment(const EHorizontalAlignment& Alignment){ 
	HorizontalAlignment = Alignment;
	return *this;
}
VerticalBoxSlot& VerticalBoxSlot::SetVerticalAlignment(const EVerticalAlignment& Alignment) {
	VerticalAlignment = Alignment;
	return *this;
}