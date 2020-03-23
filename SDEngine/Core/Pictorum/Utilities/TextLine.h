#pragma once
#include "Core/DataStructures/Array.h"
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Objects/CoreTypes/DistanceFieldFont.h"
#include "Core/Pictorum/PictorumDataTypes.h"
#include "Core/Utilities/Logger.h"

class TextLine {
public:
	TextLine(const DistanceFieldFont* Font, const float& Tracking, const float& MaxLineWidth);
	~TextLine();

	const int32& GetGlyphCount() const;
	const int32& GetCursorInteractableGlyphCount() const;
	const TString& GetText() const;
	const bool& IsEmpty() const;
	const bool& EndsWithIncompleteWord() const;
	void GetBounds(Vector2D& MinBounds, Vector2D& MaxBounds) const;
	const float GetCursorPosition() const;
	const SArray<Vector2D>& GetVerticies() const;
	const SArray<Vector2D>& GetTextureCoordinates() const;
	const SArray<int32>& GetInidices() const;

	int32 AddWord(const TString& Word, const bool& ForceFit = false);

	void Finalize();
	void Flush();
private:
	const DistanceFieldFont* Font;
	TString Text;

	float CursorPosition;
	float Tracking;
	float MaximumWidth;
	float SpaceWidth;
	float HyphenWidth;

	bool bEndsWithIncompleteWord;
	bool bFinialized;

	SArray<Vector2D> Verticies;
	SArray<Vector2D> TextureCoordinates;
	SArray<int32> Indices;

	Vector2D MinimumBounds;
	Vector2D MaximumBounds;

	void TextLine::AddCharacter(const FDistanceFieldCharacter* Character);
	const float GetCharacterWidth(const char& Character) const;
	const float GetWordWidth(const TString& Word) const;
	const int32 GetAmountOfCharactersThatFit(const TString& Word, const float& WidthLimit) const;
};

