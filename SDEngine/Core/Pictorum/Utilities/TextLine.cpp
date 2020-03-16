#include "TextLine.h"

TextLine::TextLine(const DistanceFieldFont* Font, float& Tracking) : Font(Font), Tracking(Tracking) {
	Flush();
}
TextLine::~TextLine() {
	Flush();
}
const float& TextLine::GetCursorPosition() const {
	return CursorPosition;
}
const int32 TextLine::GetLength() const {
	return Text.length() + 1;
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

			// If the character is valid, add it, otherwise just add empty verticies.
			if (dfChar) {
				AddCharacter(*dfChar);
			} else {
				Verticies.Add(Vector2D(0.0f, 0.0f));
				Verticies.Add(Vector2D(0.0f, 0.0f));
				Verticies.Add(Vector2D(0.0f, 0.0f));
				Verticies.Add(Vector2D(0.0f, 0.0f));

				TexCoords.Add(Vector2D(0.0f, 0.0f));
				TexCoords.Add(Vector2D(0.0f, 0.0f));
				TexCoords.Add(Vector2D(0.0f, 0.0f));
				TexCoords.Add(Vector2D(0.0f, 0.0f));

				Indices.Add(2);
				Indices.Add(1);
				Indices.Add(0);
				Indices.Add(3);
				Indices.Add(2);
				Indices.Add(0);
			}
		}
	} else {
		Verticies.Add(Vector2D(0.0f, 0.0f));
		Verticies.Add(Vector2D(0.0f, 0.0f));
		Verticies.Add(Vector2D(0.0f, 0.0f));
		Verticies.Add(Vector2D(0.0f, 0.0f));

		TexCoords.Add(Vector2D(0.0f, 0.0f));
		TexCoords.Add(Vector2D(0.0f, 0.0f));
		TexCoords.Add(Vector2D(0.0f, 0.0f));
		TexCoords.Add(Vector2D(0.0f, 0.0f));

		Indices.Add(2);
		Indices.Add(1);
		Indices.Add(0);
		Indices.Add(3);
		Indices.Add(2);
		Indices.Add(0);
	}
}
void TextLine::Flush() {
	CursorPosition = 0.0f;
	Verticies.Clear();
	TexCoords.Clear();
	Indices.Clear();
	Text.clear();
}
void TextLine::AddCharacter(const FDistanceFieldCharacter& Character) {
	// Add the texture coordinates.
	TexCoords.AddAll(Character.GetTextureCoordinates());

	// Add the indices.
	for (int32 i = 0; i < Character.GetIndices().Count(); i++) {
		Indices.Add(Character.GetIndices()[i] + Verticies.Count());
	}

	// Add all the verticies.
	for (int32 i = 0; i < Character.GetVerticies().Count(); i++) {
		const Vector2D& vert = Character.GetVerticies()[i];
		Verticies.Add(Vector2D(vert.x + CursorPosition, vert.y));
	}

	// Advance the cursor.
	CursorPosition += Character.GetAdvance() + Tracking;
}