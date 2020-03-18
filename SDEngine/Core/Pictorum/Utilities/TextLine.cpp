#include "TextLine.h"

TextLine::TextLine(const DistanceFieldFont* Font, const float& Tracking) : Font(Font), Tracking(Tracking) {
	Flush();
}
TextLine::~TextLine() {
	Flush();
}
const float& TextLine::GetCursorPosition() const {
	return CursorPosition;
}
const int32 TextLine::GetLength() const {
	return Text.length();
}
const TString& TextLine::GetText() const {
	return Text;
}
const SArray<Vector2D>& TextLine::GetVerticies() const {
	return Verticies;
}
const SArray<Vector2D>& TextLine::GetTextureCoordinates() const {
	return TexCoords;
}
const SArray<int32>& TextLine::GetInidices() const {
	return Indices;
}
void TextLine::SetText(const TString& LineText) {
	Text = LineText;

	// If we have real text, then add it. If the provided text is empty, still add one quad's worth
	// of data so as to keep track of this line's location.
	if (LineText.length() > 0) {
		for (const char& character : Text) {
			const FDistanceFieldCharacter* dfChar = Font->GetDistanceFieldCharacter(character);
			//if (character == '\n') {
			//	dfChar = Font->GetDistanceFieldCharacter('!');
			//}
			AddCharacter(dfChar);
		}
	} else {
		// This will add a quad with 0 size at the right bound of the previous character.
		AddCharacter(nullptr);
	}
}
void TextLine::GetBounds(Vector2D& MinBounds, Vector2D& MaxBounds) const {
	MinBounds = MinimumBounds;
	MaxBounds = MaximumBounds;
}
void TextLine::Flush() {
	CursorPosition = 0.0f;
	Verticies.Clear();
	TexCoords.Clear();
	Indices.Clear();
	Text.clear();

	MaximumBounds = Vector2D(-FLT_MAX, -FLT_MAX);
	MinimumBounds = Vector2D(FLT_MAX, FLT_MAX);
}
void TextLine::AddCharacter(const FDistanceFieldCharacter* Character) {
	if (Character) {
		// Add the texture coordinates.
		TexCoords.AddAll(Character->GetTextureCoordinates());

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
	} else {
		TexCoords.Add(Vector2D(0.0f, 0.0f));
		TexCoords.Add(Vector2D(0.0f, 0.0f));
		TexCoords.Add(Vector2D(0.0f, 0.0f));
		TexCoords.Add(Vector2D(0.0f, 0.0f));

		Indices.Add(Verticies.Count() + 2);
		Indices.Add(Verticies.Count() + 1);
		Indices.Add(Verticies.Count() + 0);
		Indices.Add(Verticies.Count() + 3);
		Indices.Add(Verticies.Count() + 2);
		Indices.Add(Verticies.Count() + 0);

		Verticies.Add(Vector2D(CursorPosition, 0.0f));
		Verticies.Add(Vector2D(CursorPosition, 0.0f));
		Verticies.Add(Vector2D(CursorPosition, 0.0f));
		Verticies.Add(Vector2D(CursorPosition, 0.0f));
	}

	// Update the bounds.
	Vector2D& bottomLeft = Verticies[Verticies.Count() - 4];
	Vector2D& topRight = Verticies[Verticies.Count() - 2];
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
}