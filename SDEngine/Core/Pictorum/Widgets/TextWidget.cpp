#include "TextWidget.h"
#include "Core/Pictorum/DistanceFieldFont.h"
#include "Core/Pictorum/TextRenderer.h"

TextWidget::TextWidget(const TString& Name) : PictorumWidget(Name) {
	Font = new DistanceFieldFont("Arial", "./Res/Fonts/Arial");
	Renderer = new TextRenderer(24, Font);
	LastRenderedAbsoluteLocation = vec2(0.0f, 0.0f);
}
TextWidget::~TextWidget() {

}
void TextWidget::SetText(const TString& Text) {
	Renderer->SetText(Text);
}
void TextWidget::SetTextColor(const FColor& Color) {
	Renderer->SetColor(Color);
}
const FColor& TextWidget::GetTextColor() {
	return Renderer->GetColor();
}
void TextWidget::SetFontSize(const int32& FontSize) {
	Renderer->SetFontSize(FontSize);
}
const int32 TextWidget::GetFontSize() {
	return Renderer->GetFontSize();
}
void TextWidget::Draw(float DeltaTime, const FRenderGeometry& Geometry) {
	vec2 location = Geometry.GetLocation(EPictorumLocationBasis::ABSOLUTE);
	location.y += Geometry.GetAllotedSpace(EPictorumScaleBasis::ABSOLUTE).y;

	// Capture the last render location with respect to the text.
	LastRenderedAbsoluteLocation = location;

	location /= Geometry.GetRenderResolution();
	location = (location - 0.5f) * 2.0f;

	Renderer->Draw(location, Geometry.GetRenderResolution(), Geometry.GetDPI());
}
vec2 TextWidget::GetDesiredDrawSpace(const FRenderGeometry& Geometry) const {
	return Renderer->GetTextBoundingBoxDimensions();
}
void TextWidget::CalculateBounds(vec2 RenderTargetResolution, vec2& MinBounds, vec2& MaxBounds) const {
	vec2 lastLocation   = LastRenderedGeometry.GetLocation(EPictorumLocationBasis::ABSOLUTE);
	MinBounds           = lastLocation;
	MinBounds.y        += LastRenderedGeometry.GetAllotedSpace(EPictorumScaleBasis::ABSOLUTE).y;
	MinBounds.y        -= Renderer->GetTextBoundingBoxDimensions().y/2;
	MaxBounds           = lastLocation;
	MaxBounds.y        += LastRenderedGeometry.GetAllotedSpace(EPictorumScaleBasis::ABSOLUTE).y;
	MaxBounds.x        += Renderer->GetTextBoundingBoxDimensions().x/2;
}
bool TextWidget::CanAddChild() const {
	return false;
}
void TextWidget::OnMouseEnter(vec2 MousePosition, FUserInterfaceEvent& Event) {
	SetTextColor(FColor(1.0f, 0.0f, 0.0));
}
void TextWidget::OnMouseExit(vec2 MousePosition, FUserInterfaceEvent& Event) {
	SetTextColor(FColor(1.0f, 1.0f, 1.0));
}
