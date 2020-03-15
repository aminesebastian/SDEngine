#pragma once
#include "Core/DataStructures/Array.h"
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Objects/CoreTypes/DistanceFieldFont.h"
#include "Core/Pictorum/PictorumDataTypes.h"
#include "Core/Utilities/Logger.h"

class TextLine {
public:
	TextLine(const DistanceFieldFont* Font, float& Tracking);
	~TextLine();

	const float& GetCursorPosition() const;
	const int32 GetLength() const;
	const TString& GetText() const;
	const SArray<Vector2D>& GetVerticies() const;
	const SArray<Vector2D>& GetTextureCoordinates() const;
	const SArray<int32>& GetInidices() const;
	void SetText(const TString& LineText);
	void Flush();
private:
	const DistanceFieldFont* Font;
	float Tracking;
	float CursorPosition;
	TString Text;
	SArray<Vector2D> Verticies;
	SArray<Vector2D> TexCoords;
	SArray<int32> Indices;

	void AddCharacter(const FDistanceFieldCharacter& Character);
};

