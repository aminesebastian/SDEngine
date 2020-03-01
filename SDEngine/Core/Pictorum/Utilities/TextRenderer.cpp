#include "TextRenderer.h"
#include "Core/Engine/EngineStatics.h"
#include "Core/Objects/CoreTypes/Shader.h"
#include "Core/Utilities/Math/MathLibrary.h"


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

	VertexArrayBuffer = new GPUVertexBufferArray();
	VertexArrayBuffer->AddBuffer("Vertex", EGPUBufferType::ArrayBuffer, EGPUBufferUsage::StaticDraw, EGPUBufferDataType::Float);
	VertexArrayBuffer->AddBuffer("TexCoord", EGPUBufferType::ArrayBuffer, EGPUBufferUsage::StaticDraw, EGPUBufferDataType::Float);
	VertexArrayBuffer->AddBuffer("Index", EGPUBufferType::ElementBuffer, EGPUBufferUsage::StaticDraw, EGPUBufferDataType::Index);
	Flush();
}
TextRenderer::~TextRenderer() {
	delete VertexArrayBuffer;
	delete TextBlockCache;
}

void TextRenderer::Draw(const Vector2D& Position, const Vector2D& RenderTargetResolution, const Vector2D& DisplayDPI) {
	if (!bBoundToGPU) {
		BindToGPU();
	}

	LastFrameScale = (FontSize / RenderTargetResolution) * (DisplayDPI / DOTS_PER_POINT);
	LastFrameMinBounds = LastFrameScale * (TextBlockCache->MinPosition) * RenderTargetResolution;
	LastFrameMaxBounds = LastFrameScale * (TextBlockCache->MaxPosition) * RenderTargetResolution;
	LastFrameRenderPosition = Position + (LastFrameScale * (TextBlockCache->MinPosition));

	Shader* fontShader = EngineStatics::GetFontShader();
	fontShader->Bind();
	Font->BindAtlas(fontShader, "Atlas", 0);

	fontShader->SetShaderVector2("LOCATION", LastFrameRenderPosition);
	fontShader->SetShaderVector2("SCALE", LastFrameScale);
	fontShader->SetShaderVector4("COLOR", Color);
	fontShader->SetShaderVector2("RENDER_TARGET_RESOLUTION", RenderTargetResolution);
	fontShader->SetShaderFloat("WIDTH", DistanceFieldWidth);
	fontShader->SetShaderFloat("EDGE", DistanceFieldEdge);

	VertexArrayBuffer->DrawTriangleElements(2, TextBlockCache->IndexCount);
}
void TextRenderer::SetText(const TString& Text) {
	this->Text = Text;
	Flush();
	AddLine(Text);
	TextBlockCache->Finalize();
}
const TString& TextRenderer::GetText() const {
	return Text;
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
const FTextBlock* TextRenderer::GetInternalDataStructure() const {
	return TextBlockCache;
}
const void TextRenderer::GetTextBoundingBoxDimensions(Vector2D& MinBounds, Vector2D& MaxBounds) const {
	MinBounds = LastFrameMinBounds;
	MaxBounds = LastFrameMaxBounds;
}
const Vector2D TextRenderer::GetCursorLocationForCharacterIndex(const int32& Index) {
	int32 internalIndex = MathLibrary::Clamp(Index, 0, (int32)Text.length());
	Vector2D vertPos = TextBlockCache->Verticies[MathLibrary::Max(0, (internalIndex * 4) - 1)];
	if (internalIndex > 0) {
		vertPos.x += Tracking;
	}
	vertPos *= LastFrameScale;
	vertPos += LastFrameRenderPosition;
	return (vertPos + 1.0f) / 2.0f;
}
const int32 TextRenderer::GetCharacterIndexAtMouseLocation(const Vector2D& MouseLocation, const Vector2D ScreenResolution) const {
	Vector2D mouseLocationAdjusted = MouseLocation / ScreenResolution;
	mouseLocationAdjusted = (mouseLocationAdjusted - 0.5f) * 2.0f;

	for (int32 i = 0; i < Text.length(); i++) {
		if (Text[i] == '\n') {
			continue;
		}
		Vector2D bottomLeft = TextBlockCache->Verticies[i * 4];
		bottomLeft *= LastFrameScale;
		bottomLeft += LastFrameRenderPosition;

		Vector2D topRight = TextBlockCache->Verticies[(i * 4) + 2];
		topRight *= LastFrameScale;
		topRight += LastFrameRenderPosition;

		if (mouseLocationAdjusted.x >= bottomLeft.x && mouseLocationAdjusted.x <= topRight.x) {
			if (mouseLocationAdjusted.y >= bottomLeft.y && mouseLocationAdjusted.y <= topRight.y) {
				return i;
			}
		}
	}
	return -1;
}
void TextRenderer::BindToGPU() {
	// Do not bind if there is no data.
	if (TextBlockCache->Verticies.Count() == 0 || TextBlockCache->TexCoords.Count() == 0 || TextBlockCache->Indices.Count() == 0) {
		SD_ENGINE_ERROR("There was an attempt to bind text geometry to the GPU where no geometry was defined!");
		return;
	}

	VertexArrayBuffer->SetBufferData(0, TextBlockCache->Verticies, TextBlockCache->VertexCount);
	VertexArrayBuffer->SetBufferData(1, TextBlockCache->TexCoords, TextBlockCache->TexCoordCount);
	VertexArrayBuffer->SetBufferData(2, TextBlockCache->Indices, TextBlockCache->IndexCount);
	VertexArrayBuffer->Update();

	bBoundToGPU = true;
}
void TextRenderer::Flush() {
	TextBlockCache->Flush();
	bBoundToGPU = false;
}
