#pragma once
#include "Core/DataStructures/Array.h"
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Pictorum/DistanceFieldFont.h"

class TextQuadBuffer {
public:
	TextQuadBuffer(int32 FontSize, const DistanceFieldFont* Font);
	void AddString(const TString& String);
	void Draw();

	void NewLine();
	void BindToGPU();

protected:
	void AddGlyph(const FDistanceFieldCharacter& Character);

private:
	bool bBoundToGPU;

	SArray<vec3> Verticies;
	SArray<vec2> TexCoords;
	SArray<uint32> Indices;

	int32 FontSize;
	const DistanceFieldFont* Font;
	int32 CurrentIndexValue;
	float CurrentXPos;
	float CurrentYPos;

	SArray<GLuint> VertexArrayBuffers;
	GLuint VertexArrayObject;
};

