#include "TextRenderer.h"
#include "Core/Engine/EngineStatics.h"
#include "Core/Objects/CoreTypes/Shader.h"
#include "Core/Utilities/Math/MathLibrary.h"
#include "Core/Utilities/StringUtilities.h"


TextRenderer::TextRenderer(int32 FontSize, const DistanceFieldFont* Font) : Font(Font) {
	TextBlockCache = new TextGeometryCache(Font, Leading, Tracking, Alignment);

	SetLeading(0.05f);
	SetTracking(-0.15f);
	SetColor(FColor(1.0f, 1.0f, 1.0f, 1.0f));
	SetFontSize(FontSize);
	SetFontWeight(EFontWeight::Normal);
	SetTextAlignment(ETextAlignment::LEFT);

	LastFrameMinBounds = ZERO_VECTOR2D;
	LastFrameMaxBounds = ZERO_VECTOR2D;

	Flush();
}
TextRenderer::~TextRenderer() {
	delete VertexArrayBuffer;
	delete TextBlockCache;
}

void TextRenderer::Draw(const Vector2D& Position, const Vector2D& RenderTargetResolution, const Vector2D& DisplayDPI) {
	if (Text.length() == 0) {
		return;
	}
	if (!bBoundToGPU) {
		BindToGPU();
	}

	CalculatedRenderScale = (FontSize / RenderTargetResolution) * (DisplayDPI / DOTS_PER_POINT);
	LastFrameMinBounds = MathLibrary::ConvertNdcToAbsoluteScreenCoordinates(CalculatedRenderScale * TextBlockCache->GetMinimumPosition(), RenderTargetResolution);
	LastFrameMaxBounds = MathLibrary::ConvertNdcToAbsoluteScreenCoordinates(CalculatedRenderScale * TextBlockCache->GetMaximumPosition(), RenderTargetResolution);
	LastFrameRenderPosition = Position - Vector2D(CalculatedRenderScale.x * TextBlockCache->GetMinimumPosition().x, -CalculatedRenderScale.y * TextBlockCache->GetMinimumPosition().y); // Subtract the minimum position as the min position is usually negative.

	Shader* fontShader = EngineStatics::GetFontShader();
	fontShader->Bind();
	Font->BindAtlas(fontShader, "Atlas", 0);

	fontShader->SetShaderVector2("LOCATION", LastFrameRenderPosition);
	fontShader->SetShaderVector2("SCALE", CalculatedRenderScale);
	fontShader->SetShaderVector4("COLOR", Color);
	fontShader->SetShaderVector2("RENDER_TARGET_RESOLUTION", RenderTargetResolution);
	fontShader->SetShaderFloat("WIDTH", DistanceFieldWidth);
	fontShader->SetShaderFloat("EDGE", DistanceFieldEdge);

	VertexArrayBuffer->DrawTriangleElements(2, TextBlockCache->GetInidices().Count());
}
void TextRenderer::BindToGPU() {
	// Do not bind if there is no data.
	if (TextBlockCache->GetVerticies().Count() == 0 || TextBlockCache->GetTextureCoordinates().Count() == 0 || TextBlockCache->GetInidices().Count() == 0) {
		return;
	}

	VertexArrayBuffer = new GPUVertexBufferArray();
	VertexArrayBuffer->AddBuffer("Vertex", EGPUBufferType::ArrayBuffer, EGPUBufferUsage::StaticDraw, EGPUBufferDataType::Float);
	VertexArrayBuffer->AddBuffer("TexCoord", EGPUBufferType::ArrayBuffer, EGPUBufferUsage::StaticDraw, EGPUBufferDataType::Float);
	VertexArrayBuffer->AddBuffer("Index", EGPUBufferType::ElementBuffer, EGPUBufferUsage::StaticDraw, EGPUBufferDataType::Index);

	VertexArrayBuffer->SetBufferData(0, TextBlockCache->GetVerticies());
	VertexArrayBuffer->SetBufferData(1, TextBlockCache->GetTextureCoordinates());
	VertexArrayBuffer->SetBufferData(2, TextBlockCache->GetInidices());
	VertexArrayBuffer->Update();

	bBoundToGPU = true;
}
void TextRenderer::Flush() {
	TextBlockCache->Flush();
	bBoundToGPU = false;
}

void TextRenderer::SetText(const TString& TextIn) {
	Flush();
	Text = TextIn;
	TextBlockCache->SetText(TextIn);
}
const TString& TextRenderer::GetText() const {
	return Text;
}
void TextRenderer::SetFontSize(const int32& Size) {
	FontSize = (float)Size / DOTS_PER_POINT;
	bBoundToGPU = false;
}
const int32 TextRenderer::GetFontSize() const {
	return (int32)(FontSize * DOTS_PER_POINT);
}
void TextRenderer::SetColor(const FColor& TextColor) {
	Color = TextColor;
}
const FColor& TextRenderer::GetColor() const {
	return Color;
}
void TextRenderer::SetTracking(const float& TextTracking) {
	Tracking = TextTracking;
	TextBlockCache->SetTracking(TextTracking);
	bBoundToGPU = false;
}
const float& TextRenderer::GetTracking() const {
	return Tracking;
}
void TextRenderer::SetLeading(const float& TextLeading) {
	Leading = TextLeading;
	TextBlockCache->SetLeading(TextLeading);
	bBoundToGPU = false;
}
const float& TextRenderer::GetLeading() const {
	return Leading;
}
void TextRenderer::SetFontWeight(const EFontWeight& Weight) {
	FontWeight = Weight;
	if (FontWeight == EFontWeight::Light) {
		DistanceFieldWidth = 0.45f;
		DistanceFieldEdge = 0.52f;
	} else if (FontWeight == EFontWeight::Normal) {
		DistanceFieldWidth = 0.47f;
		DistanceFieldEdge = 0.59f;
	} else if (FontWeight == EFontWeight::Bold) {
		DistanceFieldWidth = 0.54f;
		DistanceFieldEdge = 0.645f;
	}
}
const EFontWeight& TextRenderer::GetFontWeight() const {
	return FontWeight;
}
void TextRenderer::SetTextAlignment(const ETextAlignment& AlignmentIn) {
	Alignment = AlignmentIn;
	TextBlockCache->SetAligment(AlignmentIn);
	bBoundToGPU = false;
}
const ETextAlignment& TextRenderer::GetTextAlignment() const {
	return Alignment;
}
void TextRenderer::SetWordWrapRule(const ETextWrapRule& Rule) {
	WordWrapRule = Rule;
}
const ETextWrapRule& TextRenderer::GetWordWrapRule() const {
	return WordWrapRule;
}
void TextRenderer::SetWordWrapWidth(const float& Width) {
	float ndcWidth = Width * 2.0f;
	TextBlockCache->SetWordWrapWidth(ndcWidth / GetNdcScale().x);
	bBoundToGPU = false;
}
const float TextRenderer::GetWordWrapWidth() const {
	float ndcWidth = TextBlockCache->GetWordWrapWidth() * GetNdcScale().x;
	return ndcWidth / 2.0f;
}
const Vector2D& TextRenderer::GetNdcScale() const {
	return CalculatedRenderScale;
}
const Vector2D& TextRenderer::GetNdcPosition() const {
	return LastFrameRenderPosition;
}
const void TextRenderer::GetTextBoundingBoxDimensions(Vector2D& MinBounds, Vector2D& MaxBounds) const {
	MinBounds = LastFrameMinBounds;
	MaxBounds = LastFrameMaxBounds;
}
const void TextRenderer::GetNdcCharacterBounds(const int32& CharacterIndex, Vector2D& BottomLeft, Vector2D& TopRight) const {
	// Capture the text space coordinates.
	BottomLeft = TextBlockCache->GetVerticies()[CharacterIndex * 4];
	TopRight = TextBlockCache->GetVerticies()[(CharacterIndex * 4) + 2];

	// Get the relative coordinates.
	int32 lineIndex, relativeIndex;
	GetLineForCharacterIndex(CharacterIndex, lineIndex, relativeIndex);

	// Add the tracking if we are beyond the first character.
	if (relativeIndex > 0) {
		BottomLeft.x -= Tracking;
		if (relativeIndex < TextBlockCache->GetLine(lineIndex)->GetGlyphCount() - 1) {
			TopRight.x -= Tracking;
		}
	}

	// Transform from text space to ndc coordinates.
	BottomLeft *= CalculatedRenderScale;
	BottomLeft += LastFrameRenderPosition;
	TopRight *= CalculatedRenderScale;
	TopRight += LastFrameRenderPosition;
}
const TextGeometryCache* TextRenderer::GetGeometryCache() const {
	return TextBlockCache;
}
const int32 TextRenderer::GetMaximumIndex() const {
	int32 maxIndex = 0;
	for (const TextLine* line : TextBlockCache->GetLines()) {
		maxIndex += line->GetCursorInteractableGlyphCount();
	}
	return maxIndex - 1;
}
void TextRenderer::GetLineForCharacterIndex(const int32& AbsoluteIndex, int32& LineIndex, int32& LineRelativeCharacterIndex) const {
	LineIndex = 0;
	LineRelativeCharacterIndex = 0;
	int32 counter = AbsoluteIndex;
	int32 lineLength = 0;
	const TextLine* line = nullptr;

	for (int32 i = 0; i < TextBlockCache->GetLineCount(); i++) {
		lineLength = TextBlockCache->GetLine(i)->GetCursorInteractableGlyphCount()-1;

		if (counter >= lineLength) {
			counter -= lineLength;
		} else {
			LineIndex = i;
			LineRelativeCharacterIndex = counter;
			return;
		}
	}
}