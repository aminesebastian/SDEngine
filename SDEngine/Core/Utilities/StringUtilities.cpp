#include "StringUtilities.h"
#include "Core/DataStructures/StringBuilder.h"
#include <algorithm>
#include <sstream>

void StringUtilities::SplitString(const TString& String, const char& Delimeter, SArray<TString>& Output, const bool& IncludeDelimieter) {
	// Allocate a string builder.
	StringBuilder builder;

	// Iterate through every character. If it is not the delimiter, add it to the builder. If it is
	// the delimiter, finish the builder, add the result to the output, and clear it. If we are
	// keeping the delimiters, add a string of that as well.
	for (const char& character : String) {

		// Are we at a delimiter? If so, build the string, add it to the output, then clear the
		// builder.
		if (character == Delimeter) {
			Output.Add(builder.ToString());
			builder.Clear();

			// If we are keeping the delimiter, include that as well.
			if (IncludeDelimieter) {
				Output.Add(TString(1, character));
			}
		} else {
			// Add the character.
			builder << character;
		}
	}

	// Ensure whatever remains in the builder is also added to the output.
	if (!builder.IsEmpty()) {
		Output.Add(builder.ToString());
	}
}
void StringUtilities::SplitStringByWhitespace(const TString& String, SArray<TString>& Output) {
	std::istringstream iss(String);
	std::vector<std::string> ret((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	for (const std::string& val : ret) {
		Output.Add(val);
	}
}
const TString StringUtilities::RemoveCharacterFromString(const TString& String, const char& Character) {
	TString output = String;
	output.erase(std::remove(output.begin(), output.end(), Character), output.end());
	return output;
}
const TString StringUtilities::RemoveCharactersFromString(const TString& String, const TString& Characters) {
	TString output = String;
	for (const char& character : Characters) {
		output.erase(std::remove(output.begin(), output.end(), character), output.end());
	}
	return output;
}
const TString StringUtilities::LeftTrim(const TString& String) {
	string copy = String;
	copy.erase(copy.begin(), std::find_if(copy.begin(), copy.end(), [](int ch) {
		return !std::isspace(ch);
		}));

	return copy;
}
const TString StringUtilities::RightTrim(const TString& String) {
	string copy = String;
	copy.erase(std::find_if(copy.rbegin(), copy.rend(), [](int ch) {
		return !std::isspace(ch);
		}).base(), copy.end());

	return copy;
}
const TString StringUtilities::Trim(const TString& String) {
	string copy = String;
	copy.erase(copy.begin(), std::find_if(copy.begin(), copy.end(), [](int ch) {
		return !std::isspace(ch);
		}));
	copy.erase(std::find_if(copy.rbegin(), copy.rend(), [](int ch) {
		return !std::isspace(ch);
		}).base(), copy.end());

	return copy;
}

const TString StringUtilities::InsertAtIndex(TString& Source, const int32& Index, const TString& TextToIndex) {
	return Source.insert(Index, TextToIndex);
}
const TString StringUtilities::RemoveAtIndex(TString& Source, const int32& Index, const int32& CharacterToErase) {
	return Source.erase(Index, CharacterToErase);
}