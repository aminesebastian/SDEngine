#include "TextGeometryCache.h"
#include "Core/Utilities/Math/MathLibrary.h"
#include <limits>

TextGeometryCache::TextGeometryCache(const DistanceFieldFont* Font, const float& Leading, const float& Tracking, const ETextAlignment& Alignment) : Font(Font), Leading(Leading), Tracking(Tracking), Alignment(Alignment) {
	Flush();
}
TextGeometryCache::~TextGeometryCache() {
	Flush();
}
const int32 TextGeometryCache::GetLineCount() const {
	return Lines.Count();
}
const SArray<TextLine*>& TextGeometryCache::GetLines() const {
	return Lines;
}
const TextLine* TextGeometryCache::GetLine(const int32& Index) const {
	return Lines[Index];
}
const SArray<Vector2D>& TextGeometryCache::GetVerticies() const {
	return Verticies;
}
const SArray<Vector2D>& TextGeometryCache::GetTextureCoordinates() const {
	return TexCoords;
}
const SArray<int32>& TextGeometryCache::GetInidices() const {
	return Indices;
}
const Vector2D TextGeometryCache::GetMinimumPosition() const {
	return MinPosition;
}
const Vector2D TextGeometryCache::GetMaximumPosition() const {
	return MaxPosition;
}
void TextGeometryCache::SetAligment(const ETextAlignment& AlignmentIn) {
	Alignment = AlignmentIn;
}
void TextGeometryCache::SetTracking(const float& TrackingIn) {
	Tracking = TrackingIn;
}
void TextGeometryCache::SetLeading(const float& LeadingIn) {
	Leading = LeadingIn;
}
void TextGeometryCache::AddLine(const TString& Text) {
	TextLine* line = new TextLine(Font, Tracking);
	line->SetText(Text);
	Lines.Add(line);
}
void TextGeometryCache::Finalize() {
	// Do not finalize twice without a flush in between.
	if (bFinialized) {
		return;
	}

	// Mark as finalized.
	bFinialized = true;

	// Capture the max line length for use when calculating the alignment offset.
	float maxLength = 0.0f;
	for (const TextLine* line : Lines) {
		if (line->GetCursorPosition() > maxLength) {
			maxLength = line->GetCursorPosition();
		}
	}

	// Iterate through all the used lines and add them to the buffer.
	for (const TextLine* line : Lines) {
		// Calculate the alignment offset.
		float alignmentOffset = maxLength - line->GetCursorPosition();

		// Add texture coordinates.
		TexCoords.AddAll(line->GetTextureCoordinates());

		// Add Indices and capture the max index.
		int32 maxIndex = 0;
		for (const int32& index : line->GetInidices()) {
			Indices.Add(index + CurrentIndex);
			if (index > maxIndex) {
				maxIndex = index;
			}
		}

		// Add vertices and offset for the alignment.
		for (Vector2D vert : line->GetVerticies()) {
			if (Alignment == ETextAlignment::LEFT) {
				vert.y = vert.y + CurrentYPosition;
			} else if (Alignment == ETextAlignment::RIGHT) {
				vert.x = vert.x + alignmentOffset;
				vert.y = vert.y + CurrentYPosition;
			} else if (Alignment == ETextAlignment::CENTER) {
				vert.x = vert.x + (alignmentOffset / 2.0f);
				vert.y = vert.y + CurrentYPosition;
			}

			// Add the vertex.
			Verticies.Add(vert);

			// Cache the max and min positions of this whole block by evaluating the latest vertex.	
			if (vert.x > MaxPosition.x) {
				MaxPosition.x = vert.x;
			}
			if (vert.x < MinPosition.x) {
				MinPosition.x = vert.x;
			}
			if (-vert.y < MinPosition.y) {
				MinPosition.y = -vert.y;
			}
		}

		// Offset the cursor to the next line.
		CurrentYPosition -= Font->GetMaximumCharacterHeight();

		// Increment state keeping values.
		CurrentIndex += (maxIndex + 1);
	}
	MaxPosition.y = -CurrentYPosition;
}
void TextGeometryCache::Flush() {
	bFinialized = false;
	MaxPosition = Vector2D(-FLT_MAX, -FLT_MAX);
	MinPosition = Vector2D(FLT_MAX, FLT_MAX);
	CurrentYPosition = 0.0f;
	CurrentIndex = 0;
	Verticies.Clear();
	TexCoords.Clear();
	Indices.Clear();
	for (TextLine* line : Lines) {
		delete line;
	}
	Lines.Clear();
}