#include "ConstraintBoxWidget.h"

ConstraintBoxWidget::ConstraintBoxWidget(const TString& Name) : PictorumWidget(Name) {
	SetVisibility(EPictorumVisibilityState::SELF_HIT_TEST_INVISIBLE);
	SetSize(vec2(64, 64));
	SetMaintainAspectRatio(false);
}
bool ConstraintBoxWidget::CanAddChild() const {
	return Children.Count() == 0;
}
void ConstraintBoxWidget::CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const {

}
ConstraintBoxSlot* ConstraintBoxWidget::CreateSlotForWidget(PictorumWidget* WidgetForSlot) const {
	return new ConstraintBoxSlot();
}
ConstraintBoxSlot* ConstraintBoxWidget::AddChild(PictorumWidget* Widget) {
	return Cast<ConstraintBoxSlot>(AddChildInternal(Widget));
}

ConstraintBoxWidget& ConstraintBoxWidget::SetSize(const vec2& Size){ 
	MinimumSize = Size;
	MaximumSize = Size;
	return *this;
}
ConstraintBoxWidget& ConstraintBoxWidget::SetMinimumSize(const vec2& Size){ 
	MinimumSize = Size;
	return *this;
}
ConstraintBoxWidget& ConstraintBoxWidget::SetMaximumSize(const vec2& Size){ 
	MaximumSize = Size;
	return *this;
}
ConstraintBoxWidget& ConstraintBoxWidget::SetScaleMultiplier(const float& Multiplier){ 
	ScaleMultiplier = Multiplier;
	return *this;
}
ConstraintBoxWidget& ConstraintBoxWidget::SetMaintainAspectRatio(bool MaintainAspectRatio){ 
	bMaintainAspectRatio = MaintainAspectRatio;
	return *this;
}

const vec2& ConstraintBoxWidget::GetMinimumSize() const{ 
	return MinimumSize;
}
const vec2& ConstraintBoxWidget::GetMaximumSize() const{ 
	return MaximumSize;
}
const float& ConstraintBoxWidget::GetScaleMultiplier() const{ 
	return ScaleMultiplier;
}
const bool& ConstraintBoxWidget::GetMaintainAspectRatio() const{ 
	return bMaintainAspectRatio;
}