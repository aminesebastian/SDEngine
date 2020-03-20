#include "TextLine.h"
#include "Core/Utilities/Math/MathLibrary.h"

TextLine::TextLine(const DistanceFieldFont* Font, const float& Tracking, const float& MaxLineWidth) : Font(Font), Tracking(Tracking) {
	Flush();
	SpaceWidth = GetCharacterWidth(' ');
	HyphenWidth = GetCharacterWidth('-');
	MaximumWidth = MathLibrary::Max(0.0f, MaxLineWidth);
}
TextLine::~TextLine() {
	Flush();
}
const int32& TextLine::GetGlyphCount() const {
	return Text.length() + 1;
}
const int32& TextLine::GetCursorInteractableGlyphCount() const {
	return RawText.length() + 1;
}
const TString& TextLine::GetText() const {
	return Text;
}
const bool& TextLine::IsEmpty() const {
	return Text.empty();
}
const bool& TextLine::EndsWithIncompleteWord() const {
	return bEndsWithIncompleteWord;
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

int32 TextLine::AddWord(const TString& Word, const bool& ForceFit) {
	// If provided with an empty word, do nothing.
	if (Word.length() == 0) {
		return 0;
	}

	// If this is the first word, no need to prepend a space, otherwise, prepend a space.
	bool firstWord = IsEmpty();

	// If force fitting, charsThatFit should be the length of the Word.
	int32 charsThatFit = ForceFit ? (int32)Word.length() : 0;

	// If not being forced to fit, calculate if and how many characters we can fit.
	if (!ForceFit) {
		// Calculate how much remaining space we have.
		float remainingSpace = MaximumWidth - CursorPosition + (2.0f * Tracking);

		// If this is not the first word, compensate for the space width.
		if (!firstWord) {
			remainingSpace -= SpaceWidth;
		}

		// Calculate how many characters fit.
		for (const char& character : Word) {
			float characterWidth = GetCharacterWidth(character);
			remainingSpace -= characterWidth;
			if (remainingSpace >= 0) {
				charsThatFit++;
			} else {
				remainingSpace += characterWidth;
				break;
			}
		}

		// If the characters that fit is less than the length of the word, then we are in a hyphenating
		// situation. Reduce the count of characters that can fit by one, to account for the hyphen.
		if (charsThatFit < Word.length()) {
			charsThatFit--;
		}

		// If we can fit 0 characters, return.
		if (charsThatFit <= 0) {
			return 0;
		}
	}


	// If this is the first word, simply set this line's text to it.
	// Otherwise, append a space, followed by the word.
	if (!firstWord) {
		AddCharacter(Font->GetDistanceFieldCharacter(' '));
		RawText += " ";
	}

	// Add the characters of the word as geometry.
	for (int32 i = 0; i < charsThatFit; i++) {
		AddCharacter(Font->GetDistanceFieldCharacter(Word[i]));
		RawText += Word[i];
	}

	// Add the hyphen if needed.
	if (charsThatFit != Word.length()) {
		AddCharacter(Font->GetDistanceFieldCharacter('-'));
		bEndsWithIncompleteWord = true;
	}


	// Return how many characters fit.
	return charsThatFit;
}
void TextLine::Finalize() {
	// If this has already been finalized, skip it, otherwise mark it as finalized.
	if (bFinialized) {
		return;
	}
	bFinialized = true;

	// Add a null character to the end of the line.
	AddCharacter(nullptr);
}
void TextLine::Flush() {
	CursorPosition = 0.0f;
	Text = "";
	RawText = "";
	bFinialized = false;
	bEndsWithIncompleteWord = false;
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
		Text += Character->GetCharacter();
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
		Text += 3; // End of text character.
	}
}
const float TextLine::GetCharacterWidth(const char& Character) const {
	// Increment the cursor position.
	const FDistanceFieldCharacter* dfChar = Font->GetDistanceFieldCharacter(Character);
	if (dfChar) {
		return dfChar->GetAdvance() + Tracking;
	} else {
		return 0;
	}
}
const float TextLine::GetWordWidth(const TString& Word) const {
	// If the word is empty, return 0 width.
	if (Word.length() == 0) {
		return 0.0f;
	}

	float width = 0.0f;
	// Increment the width.
	for (const char& character : Word) {
		width += GetCharacterWidth(character);
	}

	// Compensate for the tracking.
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

		width += GetCharacterWidth(character);
		if (width >= WidthLimit) {
			return count;
		} else {
			count++;
		}
	}
	return count;
}