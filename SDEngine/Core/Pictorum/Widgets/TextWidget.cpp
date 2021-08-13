#include "TextWidget.h"
#include "Core/Objects/CoreTypes/DistanceFieldFont.h"
#include "Core/Assets/AssetManager.h"
#include "Core/Utilities/Math/MathLibrary.h"
#include "Core/Pictorum/Utilities/TextRenderer.h"

TextWidget::TextWidget(const TString& Name) : PictorumWidget(Name) {
	Font                         = Engine::GetAssetManager()->FindAsset<DistanceFieldFont>("./Res/Assets/Editor/Fonts/Arial.sasset");
	Renderer                     = new TextRenderer(24, Font);
	SetVisibility(EPictorumVisibilityState::VISIBLE);
}
TextWidget::~TextWidget() {
	delete Renderer;
}

void TextWidget::SetText(const TString& Text) {
	Renderer->SetText(Text);
	OnTextSet(Text);
}
const TString& TextWidget::GetText() const {
	return Renderer->GetText();
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
void TextWidget::SetWordWrapRule(const ETextWrapRule& Rule) {
	Renderer->SetWordWrapRule(Rule);
}
const ETextWrapRule& TextWidget::GetWordWrapRule() const {
	return Renderer->GetWordWrapRule();
}
void TextWidget::Tick(float DeltaTime, const FRenderGeometry& Geometry) {
	// Reposition such that the text always renders from the top.
	Vector2D location = Geometry.GetLocation();
	location.y += Geometry.GetAllotedSpace().y;

	// Adjust for the text alignment.
	if (Renderer->GetTextAlignment() == ETextAlignment::RIGHT) {
		Vector2D requiredSpace = GetDesiredDrawSpace(Geometry);
		location.x += Geometry.GetAllotedSpace().x - requiredSpace.x;
	} else if (Renderer->GetTextAlignment() == ETextAlignment::CENTER) {
		Vector2D requiredSpace = GetDesiredDrawSpace(Geometry);
		location.x += (Geometry.GetAllotedSpace().x - requiredSpace.x) / 2.0f;
	}

	// Translate to NDC.
	location = MathLibrary::ConvertAbsoluteToNdcScreenCoordinates(location, Geometry.GetRenderResolution());

	// Tick the text renderer.
	Renderer->Tick(DeltaTime, location, Geometry.GetRenderResolution(), Geometry.GetDPI());
}
void TextWidget::Draw(float DeltaTime, const FRenderGeometry& Geometry) {
	//FBoxDrawInstruction instruction;
	//instruction.Location = Geometry.GetLocation();
	//instruction.Size = Geometry.GetAllotedSpace();
	//instruction.BackgroundColor = FColor(0.7f, 0.1f, 1.0f, 0.25f);
	//DrawBox(Geometry, instruction);

	// Draw the text.
	Renderer->Draw();
}
vec2 TextWidget::GetDesiredDrawSpace(const FRenderGeometry& Geometry) const {
	vec2 min, max;
	Renderer->GetTextBoundingBoxDimensions(min, max);
	return max - min;
}
void TextWidget::CalculateBounds(vec2 RenderTargetResolution, vec2& MinBounds, vec2& MaxBounds) const {
	vec2 lastLocation = LastRenderedGeometry.GetLocation(EPictorumLocationBasis::ABSOLUTE);
	MinBounds = lastLocation;

	MaxBounds = lastLocation + LastRenderedGeometry.GetAllotedSpace().y;
	MaxBounds.x += LastRenderedGeometry.GetAllotedSpace().x;

	MinBounds.x = MathLibrary::Max(MinBounds.x, LastRenderedGeometry.GetMinimumClipPoint().x);
	MinBounds.y = MathLibrary::Max(MinBounds.y, LastRenderedGeometry.GetMinimumClipPoint().y);
	MaxBounds.x = MathLibrary::Min(MaxBounds.x,  LastRenderedGeometry.GetMinimumClipPoint().x + LastRenderedGeometry.GetMaximumClipPoint().x);
	MaxBounds.y = MathLibrary::Min(MaxBounds.y, LastRenderedGeometry.GetMinimumClipPoint().y + LastRenderedGeometry.GetMaximumClipPoint().y);
}
const bool TextWidget::CanAddChild() const {
	return false;
}
void TextWidget::OnRenderGeometryChanged(const FRenderGeometry& NewRenderGeometry) {
	Renderer->SetWordWrapWidth(NewRenderGeometry.GetAllotedSpace().x / NewRenderGeometry.GetRenderResolution().x);
}
void TextWidget::OnTextSet(const TString& Text) {}