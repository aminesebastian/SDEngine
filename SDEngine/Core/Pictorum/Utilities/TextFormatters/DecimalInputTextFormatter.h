#pragma once
#include "Core/Pictorum/Utilities/TextFormatters/ITextInputFormatter.h"

/**
 * A decimal input text formatter.
 *
 * @author	Amine
 * @date	3/8/2020
 */
class DecimalInputTextFormatter : public ITextInputFormatter {
public:
	DecimalInputTextFormatter() : DecimalInputTextFormatter(2) {}
	DecimalInputTextFormatter(const int32& Precision) : Precision(Precision) {}

	const bool CheckTextValidity(const TString& Text) const {
		// Trim the string.
		TString trimmedString = StringUtilities::Trim(Text);

		// If the length is 0, technically it is a valid decimal of value 0.
		if (trimmedString.length() == 0) {
			return true;
		}
		
		// Attempt to parse the string as a float, returning true if the parse is successful, false otherwise.
		const char* cString = trimmedString.c_str();
		char* endCharacter  = nullptr;
		float parsedValue  = strtof(cString, &endCharacter);

		return endCharacter != cString && *endCharacter == '\0' && parsedValue != HUGE_VALF;
	}
	const TString FormatText(const TString& Text) {
		// Trim the string.
		TString trimmedString = StringUtilities::Trim(Text);

		const char* cString = trimmedString.c_str();
		char* endCharacter = nullptr;
		float parsedValue = strtof(cString, &endCharacter);

		return StringUtilities::ToStringWithPrecision(parsedValue, Precision);
	}

private:
	int32 Precision;
};