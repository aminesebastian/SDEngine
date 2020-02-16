#include "TextRenderer.h"
#include "Core/Engine/EngineStatics.h"
#include "Core/Objects/CoreTypes/Shader.h"


TextRenderer::TextRenderer(int32 FontSize, const DistanceFieldFont* Font) : Font(Font) {
	TextBlockCache = new FTextBlock(Leading, Tracking, Alignment);

	SetLeading(0.85f);
	SetTracking(-0.15f);
	SetColor(FColor(1.0f, 1.0f, 1.0f, 1.0f));
	SetFontSize(FontSize);
	SetFontWeight(EFontWeight::Normal);
	SetTextAlignment(ETextAlignment::LEFT);

	LastFrameMinBounds = ZERO_VECTOR2D;
	LastFrameMaxBounds = ZERO_VECTOR2D;

	TestBuffer = new GPUVertexBufferArray();
	TestBuffer->AddBuffer("Vertex", EGPUBufferType::ArrayBuffer, EGPUBufferUsage::StaticDraw, EGPUBufferDataType::Float);
	TestBuffer->AddBuffer("TexCoord", EGPUBufferType::ArrayBuffer, EGPUBufferUsage::StaticDraw, EGPUBufferDataType::Float);
	TestBuffer->AddBuffer("Index", EGPUBufferType::ElementBuffer, EGPUBufferUsage::StaticDraw, EGPUBufferDataType::Index);
	Flush();
}
TextRenderer::~TextRenderer() {
	delete TestBuffer;
	delete TextBlockCache;
}

void TextRenderer::Draw(const vec2& Position, const vec2& RenderTargetResolution, const vec2& DisplayDPI) {
	if (!bBoundToGPU) {
		BindToGPU();
	}

	vec2 scale = (FontSize / RenderTargetResolution) * (DisplayDPI / DOTS_PER_POINT);

	Shader* fontShader = EngineStatics::GetFontShader();
	fontShader->Bind();
	Font->BindAtlas(fontShader, "Atlas", 0);

	fontShader->SetShaderVector2("LOCATION", Position);
	fontShader->SetShaderVector2("SCALE", scale);
	fontShader->SetShaderVector4("COLOR", Color);

	fontShader->SetShaderFloat("WIDTH", DistanceFieldWidth);
	fontShader->SetShaderFloat("EDGE", DistanceFieldEdge);

	TestBuffer->DrawTriangleElements(2, TextBlockCache->IndexCount);

	LastFrameMinBounds = scale * (TextBlockCache->MinPosition) * RenderTargetResolution;
	LastFrameMaxBounds = scale * (TextBlockCache->MaxPosition) * RenderTargetResolution;
}

void TextRenderer::SetText(const TString& Text) {
	Flush();
	AddLine(Text);
}
void TextRenderer::AddLine(const TString& Line) {
	TextBlockCache->GetCurrentLine()->SetLineSize((int32)Line.length());
	for (int32 i = 0; i < Line.length(); i++) {
		const char& character = Line[i];
		if (character == '\n') {
			TextBlockCache->CompleteLine();
		} else {
			const FDistanceFieldCharacter& dfChar = Font->GetDistanceFieldCharacter(character);
			TextBlockCache->GetCurrentLine()->AddCharacter(dfChar);
		}
	}
}

void TextRenderer::SetFontSize(const int32& Size) {
	FontSize = (float)Size / DOTS_PER_POINT;
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
}
const float& TextRenderer::GetTracking() const {
	return Tracking;
}
void TextRenderer::SetLeading(const float& TextLeading) {
	Leading = TextLeading;
	TextBlockCache->Leading = TextLeading;
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
	TextBlockCache->Alignment = AlignmentIn;
}
const ETextAlignment& TextRenderer::GetTextAlignment() const {
	return Alignment;
}
const void TextRenderer::GetTextBoundingBoxDimensions(vec2& MinBounds, vec2& MaxBounds) const {
	MinBounds = LastFrameMinBounds;
	MaxBounds = LastFrameMaxBounds;
}

void TextRenderer::BindToGPU() {
	TextBlockCache->Finalize();

	// Do not bind if there is no data.
	if (TextBlockCache->Verticies.Count() == 0 || TextBlockCache->TexCoords.Count() == 0 || TextBlockCache->Indices.Count() == 0) {
		SD_ENGINE_ERROR("There was an attempt to bind text geometry to the GPU where no geometry was defined!");
		return;
	}

	TestBuffer->SetBufferData(0, TextBlockCache->Verticies, TextBlockCache->VertexCount);
	TestBuffer->SetBufferData(1, TextBlockCache->TexCoords, TextBlockCache->TexCoordCount);
	TestBuffer->SetBufferData(2, TextBlockCache->Indices, TextBlockCache->IndexCount);
	TestBuffer->Update();

	bBoundToGPU = true;
}
void TextRenderer::Flush() {
	TextBlockCache->Flush();
	bBoundToGPU = false;
}
