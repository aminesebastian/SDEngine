#include "ConstraintBoxWidget.h"

PictorumConstraintBox::PictorumConstraintBox(const TString& Name) : PictorumWidget(Name) {
	SetVisibility(EPictorumVisibilityState::SELF_HIT_TEST_INVISIBLE);
	SetSize(vec2(64, 64));
	SetMaintainAspectRatio(false);
}
const bool PictorumConstraintBox::CanAddChild() const
{
	return Children.Count() == 0;
}
void PictorumConstraintBox::CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const {

}
Vector2D PictorumConstraintBox::GetDesiredDrawSpace(const FRenderGeometry& Geometry) {

}
ConstraintBoxSlot* PictorumConstraintBox::CreateSlotForWidget(PictorumWidget* WidgetForSlot) const {
	return new ConstraintBoxSlot();
}
ConstraintBoxSlot* PictorumConstraintBox::AddChild(PictorumWidget* Widget) {
	return Cast<ConstraintBoxSlot>(AddChildInternal(Widget));
}

PictorumConstraintBox& PictorumConstraintBox::SetSize(const vec2& Size){ 
	MinimumSize = Size;
	MaximumSize = Size;
	return *this;
}
PictorumConstraintBox& PictorumConstraintBox::SetMinimumSize(const vec2& Size){ 
	MinimumSize = Size;
	return *this;
}
PictorumConstraintBox& PictorumConstraintBox::SetMaximumSize(const vec2& Size){ 
	MaximumSize = Size;
	return *this;
}
PictorumConstraintBox& PictorumConstraintBox::SetScaleMultiplier(const float& Multiplier){ 
	ScaleMultiplier = Multiplier;
	return *this;
}
PictorumConstraintBox& PictorumConstraintBox::SetMaintainAspectRatio(bool MaintainAspectRatio){ 
	bMaintainAspectRatio = MaintainAspectRatio;
	return *this;
}

const vec2& PictorumConstraintBox::GetMinimumSize() const{ 
	return MinimumSize;
}
const vec2& PictorumConstraintBox::GetMaximumSize() const{ 
	return MaximumSize;
}
const float& PictorumConstraintBox::GetScaleMultiplier() const{ 
	return ScaleMultiplier;
}
const bool& PictorumConstraintBox::GetMaintainAspectRatio() const{ 
	return bMaintainAspectRatio;
}