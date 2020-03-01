#include "SolidWidget.h"
#include "Core/Input/InputSubsystem.h"
#include "Core/Engine/Engine.h"
#include "Core/Engine/EngineStatics.h"
#include "Core/Objects/CoreTypes/Shader.h"

SolidWidget::SolidWidget(const TString& Name) : PictorumWidget(Name) {
	DrawInstruction = new FBoxDrawInstruction();

	SetVisibility(EPictorumVisibilityState::SELF_HIT_TEST_INVISIBLE);
}
SolidWidget::~SolidWidget() {
	delete DrawInstruction;
}

void SolidWidget::Draw(float DeltaTime, const FRenderGeometry& Geometry) {
	DrawInstruction->Location = Geometry.GetLocation();
	DrawInstruction->Size = Geometry.GetAllotedSpace();
	DrawBox(Geometry, *DrawInstruction);
}
const bool SolidWidget::CanAddChild() const {
	return true;
}
void SolidWidget::CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const {
	PictorumWidget::CalculateChildRenderGeometry(CurrentRenderGeometry, OutputGeometry, ChildIndex);

	OutputGeometry.AddLocation(Padding.GetLeft(), Padding.GetBottom());
	OutputGeometry.AddAllotedSpace(-(Padding.GetLeft() + Padding.GetRight()), -(Padding.GetBottom() + Padding.GetTop()));
	OutputGeometry.SetMinimumClipPoint(OutputGeometry.GetLocation());
	OutputGeometry.SetMaximumClipPoint(OutputGeometry.GetAllotedSpace());
}
vec2 SolidWidget::GetDesiredDrawSpace(const FRenderGeometry& Geometry) const {
	vec2 output = PictorumWidget::GetDesiredDrawSpace(Geometry);
	output.x += Padding.GetLeft() + Padding.GetRight();
	output.y += Padding.GetTop() + Padding.GetBottom();
	return output;
}

const FPadding& SolidWidget::GetPadding() const {
	return Padding;
}
void SolidWidget::SetPadding(const float& AllPadding) {
	Padding.SetTop(AllPadding);
	Padding.SetBottom(AllPadding);
	Padding.SetRight(AllPadding);
	Padding.SetLeft(AllPadding);
}
void SolidWidget::SetPadding(const float& TopBottomPadding, const float& RightLeftPadding) {
	Padding.SetTop(TopBottomPadding);
	Padding.SetBottom(TopBottomPadding);
	Padding.SetRight(RightLeftPadding);
	Padding.SetLeft(RightLeftPadding);
}
void SolidWidget::SetPadding(const float& Top, const float& Right, const float& Bottom, const float& Left) {
	Padding.SetTop(Top);
	Padding.SetBottom(Bottom);
	Padding.SetRight(Right);
	Padding.SetLeft(Left);
}
void SolidWidget::SetBackgroundColor(const FColor& NewColor) {
	DrawInstruction->BackgroundColor = NewColor;
}
void SolidWidget::SetBorderRadius(const float& TopLeft, const float& TopRight, const float& BottomLeft, const float& BottomRight) {
	DrawInstruction->BorderRadius.SetTopLeftRadius(TopLeft);
	DrawInstruction->BorderRadius.SetTopRightRadius(TopRight);
	DrawInstruction->BorderRadius.SetBottomLeftRadius(BottomLeft);
	DrawInstruction->BorderRadius.SetBottomRightRadius(BottomRight);
}
void SolidWidget::OnMouseEnter(const vec2& MousePosition, FUserInterfaceEvent& Event) {
	PictorumWidget::OnMouseEnter(MousePosition, Event);
	DrawInstruction->BackgroundColor += FColor(0.1f, 0.1f, 0.1f);
}
void SolidWidget::OnMouseExit(const vec2& MousePosition, FUserInterfaceEvent& Event) {
	PictorumWidget::OnMouseExit(MousePosition, Event);
	DrawInstruction->BackgroundColor -= FColor(0.1f, 0.1f, 0.1f);
}