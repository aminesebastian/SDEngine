#include "TextWord.h"

TextWord::TextWord(const DistanceFieldFont* Font) : Font(Font){
	Length = 0;
	Text = "";
}
TextWord::~TextWord() {
	Length = 0;
	Text = "";
}

const int32& TextWord::GetLength() const {
	return Length;
}
void TextWord::SetText(const TString& WordText) {
	Text = WordText;
	Length = Text.length();
}
const float TextWord::GetWidthInTextSpace() const {
	float width = 0.0f;
	// Increment the cursor position.
	for (const char& character : Text) {
		const FDistanceFieldCharacter* dfChar = Font->GetDistanceFieldCharacter(character);
		if (dfChar) {
			width += dfChar->GetAdvance();
		}
	}
	return width;
}
const TString& TextWord::GetWordText() const {
	return Text;
}