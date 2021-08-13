#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Utilities/StringUtilities.h"

/**
* A simple text formatter interface. Implementers should ensure that the call to
* CheckTextValidity() returns true regardless of how 'ugly' the input is. Uses should call
* FormatText() after the text is determined to be valid to resolve the 'ugliness'.
* 
* Any implementation should be considered stateless and transient as it may be used across a
* multitude of users.
*
* @author	Amine
* @date	3/8/2020
*/
class ITextInputFormatter {
public:

	/**
	* Checks the provided text to ensure it matches the required format. This method should return
	* true even if the provided text may not be in the optimal format. For example, if this
	* formatter was used to format a text field as only accepting float values, both 35.00 and 35.
	* should return true for validity. The used of this formatter should then call FormatText()
	* once the text is determined to be valid to do any cosmetic formatter (for example, changing
	* the 35. to 35.0)
	*
	* @param 	Text	The text to check the format validity of.
	*
	* @returns	True if the provided text is valid, false otherwise. A call to FormatText() should be
	* 			made if true.
	*/
	virtual const bool CheckTextValidity(const TString& Text) const = 0;

	/**
	* Formats the text. This method should only be called after a call to CheckTextValidity() has
	* returned true. This method handles the pretty formatting of valid but ugly text input. For
	* example, this method should resolve the valid float format input of 35. to 35.0.
	*
	* @param 	Text	The text to format.
	*
	* @returns	The formatted text.
	*/
	virtual const TString FormatText(const TString& Text) = 0;
};