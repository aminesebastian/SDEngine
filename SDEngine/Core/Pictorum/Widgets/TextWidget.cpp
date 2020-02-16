#include "TextWidget.h"
#include "Core/Pictorum/DistanceFieldFont.h"
#include "Core/Pictorum/TextRenderer.h"

TextWidget::TextWidget(const TString& Name) : PictorumWidget(Name) {
	Font = new DistanceFieldFont("Arial", "./Res/Fonts/Arial");
	Renderer = new TextRenderer(24, Font);
	LastRenderedAbsoluteLocation = vec2(0.0f, 0.0f);

	SetVisibility(EPictorumVisibilityState::VISIBLE);
}
TextWidget::~TextWidget() {

}

void TextWidget::SetText(const TString& Text) {
	Renderer->SetText(Text);
}

void TextWidget::SetTextColor(const FColor& Color) {
	Renderer->SetColor(Color);
}
const FColor& TextWidget::GetTextColor() const {
	return Renderer->GetColor();
}
void TextWidget::SetFontSize(const int32& FontSize) {
	Renderer->SetFontSize(FontSize);
}
const int32 TextWidget::GetFontSize()  const {
	return Renderer->GetFontSize();
}
void TextWidget::SetTextAlignment(const ETextAlignment& Alignment) {
	Renderer->SetTextAlignment(Alignment);
}
const ETextAlignment& TextWidget::GetAlignment() const {
	return Renderer->GetTextAlignment();
}
void TextWidget::SetFontWeight(const EFontWeight& Weight) {
	Renderer->SetFontWeight(Weight);
}
const EFontWeight& TextWidget::GetWeight() const {
	return Renderer->GetFontWeight();
}

void TextWidget::Draw(float DeltaTime, const FRenderGeometry& Geometry) {
	vec2 location = Geometry.GetLocation(EPictorumLocationBasis::ABSOLUTE);
	location.y += Geometry.GetAllotedSpace(EPictorumScaleBasis::ABSOLUTE).y;

	if (GetAlignment() == ETextAlignment::RIGHT) {
		location.x = Geometry.GetAllotedSpace().x - GetDesiredDrawSpace(Geometry).x;
	}


	// Capture the last render location with respect to the text.
	LastRenderedAbsoluteLocation = location;

	location /= Geometry.GetRenderResolution();
	location = (location - 0.5f) * 2.0f;

	Renderer->Draw(location, Geometry.GetRenderResolution(), Geometry.GetDPI());
}
vec2 TextWidget::GetDesiredDrawSpace(const FRenderGeometry& Geometry) const {
	vec2 min, max;
	Renderer->GetTextBoundingBoxDimensions(min, max);
	return max;
}
void TextWidget::CalculateBounds(vec2 RenderTargetResolution, vec2& MinBounds, vec2& MaxBounds) const {
	vec2 min, max;
	Renderer->GetTextBoundingBoxDimensions(min, max);

	vec2 lastLocation = LastRenderedGeometry.GetLocation(EPictorumLocationBasis::ABSOLUTE);
	MinBounds         = lastLocation;
	MinBounds.y      += LastRenderedGeometry.GetAllotedSpace(EPictorumScaleBasis::ABSOLUTE).y;
	MinBounds.y      -= max.y;
	MaxBounds         = MinBounds;
	MaxBounds.x      += max.x;
	MaxBounds.y       = lastLocation.y;
}
const bool TextWidget::CanAddChild() const {
	return false;
}

void TextWidget::OnMouseEnter(vec2 MousePosition, FUserInterfaceEvent& EventIn) {
	Renderer->SetColor(FColor(1.0f, 0.0f, 0.0f));
}
void TextWidget::OnMouseExit(vec2 MousePosition, FUserInterfaceEvent& EventIn) {
	Renderer->SetColor(FColor(1.0f, 1.0f, 1.0f));
}