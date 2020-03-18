#pragma once
#include "Core/Objects/CoreTypes/DistanceFieldFont.h"

class TextWord {
public:
	TextWord(const DistanceFieldFont* Font);
	~TextWord();

	/**
	* Sets the text
	*
	* @param 	WordText	The word text.
	*/

	void SetText(const TString& WordText);

	/**
	 * Gets the length of the word.
	 *
	 * @returns	The length.
	 */

	const int32& GetLength() const;
	/**
	 * Gets width of the word in text space (not including tracking, which should be considered by
	 * adding WordLength * Tracking.
	 *
	 * @returns	The width in text space.
	 */
	const float GetWidthInTextSpace() const;
	/**
	 * Gets word text
	 *
	 * @returns	The word text.
	 */
	const TString& GetWordText() const;
private:
	/** The text */
	TString Text;
	/** The length of the text (cached). */
	int32 Length;
	/** The font to use.*/
	const DistanceFieldFont* Font;
};

