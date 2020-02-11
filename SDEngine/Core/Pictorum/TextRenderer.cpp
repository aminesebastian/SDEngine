#include "TextRenderer.h"
#include "Core/Engine/EngineStatics.h"
#include "Core/Objects/CoreTypes/Shader.h"


TextRenderer::TextRenderer(int32 FontSize, const DistanceFieldFont* Font) : Font(Font) {
	TextBlockCache = new FTextBlock(Leading, Tracking, Alignment);

	SetLeading(0.75f);
	SetTracking(-0.15f);
	SetColor(FColor(1.0f, 1.0f, 1.0f, 1.0f));
	SetFontSize(FontSize);
	SetFontWeight(EFontWeight::Normal);
	SetTextAlignment(ETextAlignment::LEFT);
	Reset();
}
TextRenderer::~TextRenderer() {
	glDeleteVertexArrays(1, &VertexArrayObject);
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

	glBindVertexArray(VertexArrayObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VertexArrayBuffers[2]);
	glDrawElements(GL_TRIANGLES, TextBlockCache->Indices.Count(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	LastBoundingBoxDimensions = scale * (TextBlockCache->MaxPosition - TextBlockCache->MinPosition) * RenderTargetResolution;
}

void TextRenderer::SetText(const SArray<TString>& TextArray, bool bNewLinePerEntry) {
	Reset();
	for (const TString& line : TextArray) {
		for (char character : line) {
			if (character == '\n') {
				NewLine();
			} else {
				AddGlyph(Font->GetDistanceFieldCharacter(character));
			}
		}
		if (bNewLinePerEntry) {
			NewLine();
		}
	}
}
void TextRenderer::SetText(const TString& Text) {
	Reset();
	for (char character : Text) {
		if (character == '\n') {
			NewLine();
		} else {
			AddGlyph(Font->GetDistanceFieldCharacter(character));
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
	TextBlockCache->Tracking = TextTracking;
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
const vec2& TextRenderer::GetTextBoundingBoxDimensions() const {
	return LastBoundingBoxDimensions;
}

void TextRenderer::AddGlyph(const FDistanceFieldCharacter& Character) {
	TextBlockCache->GetCurrentLine()->AddCharacter(Character);
	bBoundToGPU = false;
}
void TextRenderer::BindToGPU() {
	TextBlockCache->Finalize();

	// Do not bind if there is no data.
	if (TextBlockCache->Verticies.Count() == 0 || TextBlockCache->TexCoords.Count() == 0 || TextBlockCache->Indices.Count() == 0) {
		SD_ENGINE_ERROR("There was an attempt to bind text geometry to the GPU where no geometry was defined!");
		return;
	}

	glGenVertexArrays(1, &VertexArrayObject);
	glBindVertexArray(VertexArrayObject);

	// Make sure there are three buffers
	while (VertexArrayBuffers.Count() < 3) {
		VertexArrayBuffers.Add(0);
	}

	glGenBuffers(3, &VertexArrayBuffers[0]);

	// Bind vertices
	glBindBuffer(GL_ARRAY_BUFFER, VertexArrayBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TextBlockCache->Verticies[0]) * TextBlockCache->Verticies.Count(), &TextBlockCache->Verticies[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Bind texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, VertexArrayBuffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TextBlockCache->TexCoords[0]) * TextBlockCache->TexCoords.Count(), &TextBlockCache->TexCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Bind indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VertexArrayBuffers[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(TextBlockCache->Indices[0]) * TextBlockCache->Indices.Count(), &TextBlockCache->Indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	bBoundToGPU = true;
}
void TextRenderer::Reset() {
	glDeleteVertexArrays(1, &VertexArrayObject);
	TextBlockCache->Reset();
	bBoundToGPU = false;
}
void TextRenderer::NewLine() {
	TextBlockCache->CompleteLine();
}
