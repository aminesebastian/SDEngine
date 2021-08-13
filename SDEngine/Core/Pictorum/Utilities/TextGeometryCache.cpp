#include "TextGeometryCache.h"
#include "Core/Utilities/Math/MathLibrary.h"
#include "Core/Utilities/StringUtilities.h"
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
const float TextGeometryCache::GetLineYOffset(const int32& Index) const {
	return (Index + 1) * Font->GetMaximumCharacterHeight();
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
void TextGeometryCache::SetWordWrapWidth(const float& Width) {
	WordWrapWidth = Width;
	SetText(TString(Text)); // Make a copy of the Text as the call to SetText resets it.
}
const float& TextGeometryCache::GetWordWrapWidth() const {
	return WordWrapWidth;
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
void TextGeometryCache::SetText(const TString& TextIn) {
	Flush();
	Text = TextIn;

	// Generate a list of the words. 
	SStack<TString> words;
	GenerateWords(Text, words);

	// Keep track of how many words we have populated into the cache. When this == the last index
	// of 'words', we have finished populating.
	int32 wordCount = 0;

	TextLine* currentLine = GetNewLine();

	while (!words.IsEmpty()) {
		const TString word = words.Peek();
		// Skip empty words.
		if (word.empty()) {
			words.Pop();
			continue;
		}

		// If this is a new line, add it and then create a new line.
		if (word == "\n") {
			words.Pop();
			currentLine->AddWord("\n", true);
			currentLine = GetNewLine();
			continue;
		}

		// Attempt to add the word. If only partially added, add the remaining back to the stack.
		int32 addedChars = currentLine->AddWord(word);
		if (addedChars > 0) {
			words.Pop();
			if (addedChars < word.length()) {
				words.Push(word.substr(addedChars));
				currentLine = GetNewLine();
			}
		} else {
			// This condition will only occur if we have a word TOO big to fit on a single line by itself.
			if (currentLine->IsEmpty()) {
				currentLine->AddWord(word, true);
				words.Pop();
			}
			// If the above condition was met, check to see if there are still more words left to add.
			if (!words.IsEmpty()) {
				currentLine = GetNewLine();
			}
		}
	}

	// Finalize the last line again JUST IN CASE. If it has already been finalized, this call will do nothing.
	currentLine->Finalize();
	Finalize();
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
	for (int32 i = 0; i < Lines.Count(); i++) {
		// Get the line.
		const TextLine* line = Lines[i];

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
			if (Alignment == ETextAlignment::RIGHT) {
				vert.x = vert.x + alignmentOffset;
			} else if (Alignment == ETextAlignment::CENTER) {
				vert.x = vert.x + (alignmentOffset / 2.0f);
			}

			vert.y += CurrentYPosition;

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
		CurrentYPosition = -GetLineYOffset(i);

		// Increment state keeping values.
		CurrentIndex += (maxIndex + 1);
	}
	MaxPosition.y = -CurrentYPosition;
}
TextLine* TextGeometryCache::GetNewLine() {
	if (Lines.Count() > 0) {
		Lines[Lines.LastIndex()]->Finalize();
	}
	TextLine* newLine = new TextLine(Font, Tracking, WordWrapRule, WordWrapWidth);
	Lines.Add(newLine);
	return newLine;
}
void TextGeometryCache::GenerateWords(const TString& SourceText, SStack<TString>& Words) const {
	// First split the string into 'words' through whitespace.
	SArray<TString> initialWords;
	StringUtilities::SplitString(SourceText, ' ', initialWords, true);

	// Then, for each 'word', separate out any newline characters and split that string into sub
	// words.
	for (const TString& compoundWord : initialWords) {
		SArray<TString> newLineSplit;
		StringUtilities::SplitString(compoundWord, '\n', newLineSplit, true);
		for (int i = 0; i < newLineSplit.Count(); i++) {
			Words.Push(newLineSplit[i]);
		}
	}
	Words.Reverse();
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
	Text = "";
	for (TextLine* line : Lines) {
		delete line;
	}
	Lines.Clear();
}
void TextGeometryCache::SetWordWrapRule(const ETextWrapRule& Rule) {
	WordWrapRule = Rule;
}
const ETextWrapRule& TextGeometryCache::GetWordWrapRule() const {
	return WordWrapRule;
}