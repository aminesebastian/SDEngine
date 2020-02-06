#include "HorizontalBoxWidget.h"

HorizontalBoxWidget::HorizontalBoxWidget(const TString& Name) : PictorumWidget(Name) {
	SetVisibility(EPictorumVisibilityState::SELF_HIT_TEST_INVISIBLE);
}
bool HorizontalBoxWidget::CanAddChild() const {
	return true;
}
void HorizontalBoxWidget::CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const {
	vec2 sizePerChild = vec2(CurrentRenderGeometry.GetAllotedSpace(EPictorumScaleBasis::ABSOLUTE).x / Children.Count(), CurrentRenderGeometry.GetAllotedSpace(EPictorumScaleBasis::ABSOLUTE).y);
	OutputGeometry.SetAllotedSpace(sizePerChild);
	OutputGeometry.SetLocation(CurrentRenderGeometry.GetLocation(EPictorumLocationBasis::ABSOLUTE) + vec2(sizePerChild.x * ChildIndex, 0.0f));
}