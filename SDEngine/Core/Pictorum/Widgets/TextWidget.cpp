#include "TextWidget.h"
#include "Core/Pictorum/DistanceFieldFont.h"
#include "Core/Rendering/TextRenderer.h"

TextWidget::TextWidget(const TString& Name) : PictorumWidget(Name) {
	Font     = new DistanceFieldFont("Arial", "./Res/Fonts/Arial");
	Renderer = new TextRenderer(24, Font);
}
TextWidget::~TextWidget() {

}
void TextWidget::SetText(const TString& Text) {
	Renderer->SetText(Text);
}

void TextWidget::Draw(float DeltaTime, const FRenderGeometry& Geometry) {
	Renderer->Draw(Geometry.GetLocation(EPictorumLocationBasis::NDC), Geometry.GetRenderResolution(), Geometry.GetDPI());
}
vec2 TextWidget::GetDesiredDrawSpace(const FRenderGeometry& Geometry) const {
	return vec2(0.0f, 0.0f);
}
void TextWidget::CalculateBounds(vec2 RenderTargetResolution, vec2& MinBounds, vec2& MaxBounds) const {

}
bool TextWidget::CanAddChild() const {
	return false;
}
void TextWidget::SetFontSize(const int32& FontSize) {
	Renderer->SetFontSize(FontSize);
}
const int32 TextWidget::GetFontSize() {
	return Renderer->GetFontSize();
}