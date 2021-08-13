#include "SeparatorWidget.h"

SeparatorWidget::SeparatorWidget(const TString& Name) : PictorumWidget(Name){
	Size.x = 0.0f;
	Size.y = 0.0f;
}
SeparatorWidget::~SeparatorWidget() {

}
vec2 SeparatorWidget::GetDesiredDrawSpace(const FRenderGeometry& Geometry) const {
	return Size;
}
void SeparatorWidget::SetSize(const float& Width, const float& Height) {
	Size = vec2(Width, Height);
}
const vec2& SeparatorWidget::GetSize() const {
	return Size;
}