#include "TextLine.h"

TextLine::TextLine(const DistanceFieldFont* Font, const float& Tracking, const float& MaximumWidth) : Font(Font), Tracking(Tracking), MaximumWidth(MaximumWidth) {
	Flush();
	SpaceWidth = GetWordWidthInTextSpace(" ");
}
TextLine::~TextLine() {
	Flush();
}
const int32& TextLine::GetLength() const {
	return Text.length();
}
const TString& TextLine::GetText() const {
	return Text;
}
void TextLine::GetBounds(Vector2D& MinBounds, Vector2D& MaxBounds) const {
	MinBounds = MinimumBounds;
	MaxBounds = MaximumBounds;
}
const float TextLine::GetCursorPosition() const {
	return CursorPosition;
}
const SArray<Vector2D>& TextLine::GetVerticies() const {
	return Verticies;
}
const SArray<Vector2D>& TextLine::GetTextureCoordinates() const {
	return TextureCoordinates;
}
const SArray<int32>& TextLine::GetInidices() const {
	return Indices;
}

bool TextLine::AddWord(const TString& Word, const bool& ForceFit) {
	// If this is the first word, no need to prepend a space, otherwise, prepend a space.
	bool firstWord = GetLength() == 0;

	// If this word is not request to force fit, get the width of the word in text space. If the
	// word will not fit, return false.
	if (!ForceFit) {
		float wordWidth = GetWordWidthInTextSpace(Word);
		if (CursorPosition + (firstWord ? 0.0f : SpaceWidth) + wordWidth > MaximumWidth) {
			return false;
		}
	}

	// If this is the first word, simply set this line's text to it.
	// Otherwise, append a space, followed by the word.
	if (firstWord) {
		Text = Word;
	} else {
		AddCharacter(Font->GetDistanceFieldCharacter(' '));
		Text += (' ' + Word);
	}

	// Add the characters of the word as geometry.
	for (const char& character : Word) {
		AddCharacter(Font->GetDistanceFieldCharacter(character));
	}

	return true;
}
void TextLine::Finalize() {
	// If this has already been finalized, skip it, otherwise mark it as finalized.
	if (bFinialized) {
		return;
	}
	bFinialized = true;

	// Add a null character to the end of the line.
	Text += '\0';
	AddCharacter(nullptr);
}
void TextLine::Flush() {
	CursorPosition = 0.0f;
	Text = "";
	bFinialized = false;
}
void TextLine::AddCharacter(const FDistanceFieldCharacter* Character) {
	if (Character) {
		// Add the texture coordinates.
		TextureCoordinates.AddAll(Character->GetTextureCoordinates());

		// Add the indices.
		for (int32 i = 0; i < Character->GetIndices().Count(); i++) {
			Indices.Add(Character->GetIndices()[i] + Verticies.Count());
		}

		// Add all the verticies.
		for (int32 i = 0; i < Character->GetVerticies().Count(); i++) {
			const Vector2D& vert = Character->GetVerticies()[i];
			Verticies.Add(Vector2D(vert.x + CursorPosition, vert.y));
		}

		// Advance the cursor.
		CursorPosition += Character->GetAdvance() + Tracking;

		// Update the bounds.
		Vector2D& bottomLeft = Verticies[(int64)Verticies.Count() - 4];
		Vector2D& topRight = Verticies[(int64)Verticies.Count() - 2];
		if (topRight.x > MaximumBounds.x) {
			MaximumBounds.x = topRight.x;
		}
		if (topRight.y > MaximumBounds.y) {
			MaximumBounds.y = topRight.y;
		}
		if (bottomLeft.x < MinimumBounds.x) {
			MinimumBounds.x = bottomLeft.x;
		}
		if (bottomLeft.y < MinimumBounds.y) {
			MinimumBounds.y = bottomLeft.y;
		}
	} else {
		TextureCoordinates.Add(Vector2D(0.0f, 0.0f));
		TextureCoordinates.Add(Vector2D(0.0f, 0.0f));
		TextureCoordinates.Add(Vector2D(0.0f, 0.0f));
		TextureCoordinates.Add(Vector2D(0.0f, 0.0f));

		Indices.Add(Verticies.Count() + 2);
		Indices.Add(Verticies.Count() + 1);
		Indices.Add(Verticies.Count() + 0);
		Indices.Add(Verticies.Count() + 3);
		Indices.Add(Verticies.Count() + 2);
		Indices.Add(Verticies.Count() + 0);

		Verticies.Add(Vector2D(MaximumBounds.x, 0.0f));
		Verticies.Add(Vector2D(MaximumBounds.x, 0.0f));
		Verticies.Add(Vector2D(MaximumBounds.x, 0.0f));
		Verticies.Add(Vector2D(MaximumBounds.x, 0.0f));
	}
}
const float TextLine::GetWordWidthInTextSpace(const TString& Word) const {
	// If the word is empty, return 0 width.
	if (Word.length() == 0) {
		return 0.0f;
	}

	float width = 0.0f;
	// Increment the cursor position.
	for (const char& character : Word) {
		const FDistanceFieldCharacter* dfChar = Font->GetDistanceFieldCharacter(character);
		if (dfChar) {
			width += dfChar->GetAdvance() + Tracking;
		}
	}

	// If the last character has dimensions, add it to the width.
	const FDistanceFieldCharacter* lastChar = Font->GetDistanceFieldCharacter(Word[Word.length() - 1]);
	if (lastChar) {
		return width - (Tracking / 2.0f);
	} else {
		return width;
	}
}
const int32 TextLine::GetAmountOfCharactersThatFit(const TString& Word, const float& WidthLimit) const {
	float width = 0.0f;
	int32 count = 0;

	// Count how many characters can fit.
	for (const char& character : Word) {
		const FDistanceFieldCharacter* dfChar = Font->GetDistanceFieldCharacter(character);
		if (dfChar) {
			width += dfChar->GetAdvance() + Tracking;
		}
		if (width = WidthLimit) {
			return count;
		} else {
			count++;
		}
	}
	return count;
}