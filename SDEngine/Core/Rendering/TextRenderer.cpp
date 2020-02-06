#include "TextRenderer.h"
#include "Engine/EngineStatics.h"
#include "Rendering/Shader.h"

TextRenderer::TextRenderer(int32 FontSize, const DistanceFieldFont* Font) : Font(Font) {
	CurrentIndexValue  = 0;
	CursorPosition     = vec2(0.0f,0.0f);
	SpaceWidth         = 0.05f;
	bBoundToGPU        = false;
	VertexArrayObject  = 0;

	VertexArrayBuffers.Add(0);
	VertexArrayBuffers.Add(0);
	VertexArrayBuffers.Add(0);

	SetLeading(0.125f);
	SetTracking(-0.15f);
	SetColor(FColor(1.0f, 1.0f, 1.0f, 1.0f));
	SetFontSize(FontSize);
	SetFontWeight(EFontWeight::Normal);
}
TextRenderer::~TextRenderer() {
	glDeleteVertexArrays(1, &VertexArrayObject);
}

void TextRenderer::SetText(const SArray<TString>& TextArray, bool bNewLinePerEntry) {
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
void TextRenderer::Draw(const vec2& Position, const vec2& RenderTargetResolution, const vec2& DisplayDPI) {
	if (!bBoundToGPU) {
		BindToGPU();
	}

	vec2 scale = (FontSize / RenderTargetResolution) * (DisplayDPI/DOTS_PER_POINT);

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
	glDrawElements(GL_TRIANGLES, Indices.Count(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void TextRenderer::SetFontSize(const int32& Size) {
	FontSize = (float)Size / DOTS_PER_POINT;
}
const int32 TextRenderer::GetFontSize() const {
	return (int32)(FontSize * DOTS_PER_POINT);
}
void TextRenderer::SetColor(FColor TextColor) {
	Color = TextColor;
}
const FColor& TextRenderer::GetColor() const {
	return Color;
}
void TextRenderer::SetTracking(const float& TextTracking) {
	Tracking = TextTracking;
}
const float& TextRenderer::GetTracking() const {
	return Tracking;
}
void TextRenderer::SetLeading(const float& TextLeading) {
	Leading = TextLeading;
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
		DistanceFieldEdge = 0.57f;
	} else if (FontWeight == EFontWeight::Bold) {
		DistanceFieldWidth = 0.54f;
		DistanceFieldEdge = 0.645f;
	}
}
const EFontWeight& TextRenderer::GetFontWeight() const {
	return FontWeight;
}
const float TextRenderer::GetMaxHorizontalPosition() {
	return MaxHorizontalPosition;
}

void TextRenderer::AddGlyph(const FDistanceFieldCharacter& Character) {
	if (CursorPosition.x == 0) {
		CursorPosition.x += Character.GetDimensions().x / 4.0f;
	}

	// Only add a quad if the character is not a space, otherwise just advance the cursor position.
	if (Character.GetCharacter() != ' ') {
		vec2 minTexCoords = Character.GetMinTextureCoords();
		vec2 maxTexCoords = Character.GetMaxTextureCoords();
		vec2 glyphSize = Character.GetDimensions();
		vec2 topLeftVert = CursorPosition + vec2(Character.GetOffsets().x, -Character.GetOffsets().y);

		TexCoords.Add(vec2(minTexCoords.x, maxTexCoords.y));
		TexCoords.Add(minTexCoords);
		TexCoords.Add(vec2(maxTexCoords.x, minTexCoords.y));
		TexCoords.Add(maxTexCoords);

		Verticies.Add(topLeftVert + vec2(0.0, -glyphSize.y));
		Verticies.Add(topLeftVert);
		Verticies.Add(topLeftVert + vec2(glyphSize.x, 0.0f));
		Verticies.Add(topLeftVert + vec2(glyphSize.x, -glyphSize.y));

		Indices.Add(CurrentIndexValue);
		Indices.Add(CurrentIndexValue + 1);
		Indices.Add(CurrentIndexValue + 2);
		Indices.Add(CurrentIndexValue);
		Indices.Add(CurrentIndexValue + 2);
		Indices.Add(CurrentIndexValue + 3);

		CurrentIndexValue += 4;
	}

	CursorPosition.x  += (Character.GetAdvance() + Tracking);

	// Cache max horizontal position
	if (CursorPosition.x > MaxHorizontalPosition) {
		MaxHorizontalPosition = CursorPosition.x;
	}

	// Mark this buffer as needing update.
	bBoundToGPU        = false;
}
void TextRenderer::BindToGPU() {
	glGenVertexArrays(1, &VertexArrayObject);
	glBindVertexArray(VertexArrayObject);

	glGenBuffers(3, &VertexArrayBuffers[0]);

	// Bind vertices
	glBindBuffer(GL_ARRAY_BUFFER, VertexArrayBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Verticies[0]) * Verticies.Count(), &Verticies[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Bind texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, VertexArrayBuffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.Count(), &TexCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Bind indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VertexArrayBuffers[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.Count(), &Indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	bBoundToGPU = true;
}

void TextRenderer::Reset() {
	glDeleteVertexArrays(1, &VertexArrayObject);
	Verticies.Clear();
	TexCoords.Clear();
	Indices.Clear();
	CursorPosition.x      = 0.0f;
	CursorPosition.y      = 0.0f;
	MaxHorizontalPosition = 0.0f;
	CurrentIndexValue     = 0;
	bBoundToGPU           = false;
}
void TextRenderer::NewLine() {
	CursorPosition.x = 0.0f;
	CursorPosition.y -= FontSize * Leading;
}
