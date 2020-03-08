#pragma once
#include "Core/Pictorum/Utilities/TextFormatters/ITextInputFormatter.h"

/**
 * An integer input text formatter.
 *
 * @author	Amine
 * @date	3/8/2020
 */
class IntegerInputTextFormatter : public ITextInputFormatter {
public:
	const bool CheckTextValidity(const TString& Text) const {
		// Trim the string.
		TString trimmedString = StringUtilities::Trim(Text);

		// If the length is 0, technically it is a valid integer of value 0.
		if (trimmedString.length() == 0) {
			return true;
		}
		
		// Attempt to parse the string as an int, returning true if the parse is successful, false otherwise.
		const char* cString = trimmedString.c_str();
		char* endCharacter = nullptr;
		int32 parsedValue = strtol(cString, &endCharacter, 10);

		return endCharacter != cString && *endCharacter == '\0';
	}
	const TString FormatText(const TString& Text) {
		// Trim the string.
		TString trimmedString = StringUtilities::Trim(Text);

		const char* cString = trimmedString.c_str();
		char* endCharacter = nullptr;
		int32 parsedValue = strtol(cString, &endCharacter, 10);

		return to_string(parsedValue);
	}
};