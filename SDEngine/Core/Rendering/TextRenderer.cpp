#include "TextRenderer.h"
#include "Engine/EngineStatics.h"
#include "Rendering/Shader.h"

TextRenderer::TextRenderer(int32 FontSize, const DistanceFieldFont* Font) : FontSize(FontSize), Font(Font) {
	CurrentIndexValue  = 0;
	CursorPosition.x   = 0.0f;
	CursorPosition.y   = 0.0f;
	Leading	           = 0.0045f;
	SpaceWidth         = 0.0005f;
	Tracking           = -0.005f;
	Color              = FColor(1.0f, 1.0f, 1.0f, 1.0f);
	bBoundToGPU        = false;
	VertexArrayObject  = 0;

	VertexArrayBuffers.Add(0);
	VertexArrayBuffers.Add(0);
	VertexArrayBuffers.Add(0);

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
void TextRenderer::Draw(const vec2& Position, const vec2& RenderTargetResolution) {
	if (!bBoundToGPU) {
		BindToGPU();
	}

	vec2 scale = (FontSize / RenderTargetResolution) * 36.0f;

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
	FontSize = (float)Size;
}
const int32& TextRenderer::GetFontSize() const {
	return (float)FontSize;
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
		DistanceFieldEdge = 0.645;
	}
}
const EFontWeight& TextRenderer::GetFontWeight() const {
	return FontWeight;
}

void TextRenderer::AddGlyph(const FDistanceFieldCharacter& Character) {
	if (Character.GetCharacter() == ' ') {
		CursorPosition.x += SpaceWidth * FontSize;
		return;
	}

	vec2 minTexCoords = Character.GetMinTextureCoords();
	vec2 maxTexCoords = Character.GetMaxTextureCoords();
	vec2 glyphSize    = Character.GetDimensions();
	vec2 topLeftVert  = CursorPosition + vec2(Character.GetOffsets().x, -Character.GetOffsets().y);

	TexCoords.Add(vec2(minTexCoords.x, maxTexCoords.y));
	TexCoords.Add(minTexCoords);
	TexCoords.Add(vec2(maxTexCoords.x, minTexCoords.y));
	TexCoords.Add(vec2(maxTexCoords.x, maxTexCoords.y));

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
	CursorPosition.x  += (Character.GetAdvance() + Tracking);
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
	CursorPosition.x  = 0.0f;
	CursorPosition.y  = 0.0f;
	CurrentIndexValue = 0;
	bBoundToGPU       = false;
}
void TextRenderer::NewLine() {
	CursorPosition.x = 0.0f;
	CursorPosition.y -= (float)FontSize * Leading;
}
