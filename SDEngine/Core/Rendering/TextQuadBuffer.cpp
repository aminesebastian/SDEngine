#include "TextQuadBuffer.h"

TextQuadBuffer::TextQuadBuffer(int32 FontSize, const DistanceFieldFont* Font) : FontSize(FontSize), Font(Font) {
	CurrentIndexValue = 0;
	CurrentXPos       = 0.0f;
	CurrentYPos       = 0.0f;

	VertexArrayBuffers.Add(0);
	VertexArrayBuffers.Add(0);
	VertexArrayBuffers.Add(0);

	bBoundToGPU       = false;
	VertexArrayObject = 0;
}
void TextQuadBuffer::AddString(const TString& String) {
	for (char character : String) {
		AddGlyph(Font->GetDistanceFieldCharacter(character));
	}
}
void TextQuadBuffer::Draw() {
	if (!bBoundToGPU) {
		BindToGPU();
	}

	glBindVertexArray(VertexArrayObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VertexArrayBuffers[2]);
	glDrawElements(GL_TRIANGLES, Indices.Count(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
void TextQuadBuffer::NewLine() {

}
void TextQuadBuffer::BindToGPU() {
	glGenVertexArrays(1, &VertexArrayObject);
	glBindVertexArray(VertexArrayObject);

	glGenBuffers(3, &VertexArrayBuffers[0]);

	// Bind verticies.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Verticies[0]) * Verticies.Count(), &Verticies[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Bind texture coordinates.
	glBindBuffer(GL_ARRAY_BUFFER, 1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.Count(), &TexCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Bind indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.Count(), &Indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	bBoundToGPU = true;
}

void TextQuadBuffer::AddGlyph(const FDistanceFieldCharacter& Character) {
	vec2 texCoords = Character.GetTextureCoords();
	vec2 glyphSize = Character.GetDimensions();

	TexCoords.Add(texCoords);
	TexCoords.Add(texCoords + vec2(0.0f, glyphSize.y));
	TexCoords.Add(texCoords + vec2(glyphSize.x, glyphSize.y));
	TexCoords.Add(texCoords + vec2(glyphSize.x, 0.0f));

	vec3 bottomLeftVert = vec3(CurrentXPos, 0.0f, 0.0f);

	float widthOffset = Character.GetAspectRatio() * FontSize;
	float heightOffset = FontSize / Character.GetAspectRatio();

	Verticies.Add(bottomLeftVert);
	Verticies.Add(bottomLeftVert + vec3(0.0f, heightOffset, 0.0f));
	Verticies.Add(bottomLeftVert + vec3(widthOffset, heightOffset, 0.0f));
	Verticies.Add(bottomLeftVert + vec3(widthOffset, 0.0f, 0.0f));

	Indices.Add(CurrentIndexValue);
	Indices.Add(CurrentIndexValue + 1);
	Indices.Add(CurrentIndexValue + 2);
	Indices.Add(CurrentIndexValue);
	Indices.Add(CurrentIndexValue + 2);
	Indices.Add(CurrentIndexValue + 3);

	CurrentIndexValue += 3;
	CurrentXPos += Character.GetAdvance() * FontSize;

	bBoundToGPU = false;
}