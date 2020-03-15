#include "StringUtilities.h"
#include <algorithm>
#include <sstream>

void StringUtilities::SplitString(const TString& String, const char& Delimeter, SArray<TString>& Output) {
	std::stringstream stringStream(String);
	TString splitItem;
	while (std::getline(stringStream, splitItem, Delimeter)) {
		Output.Add(splitItem);
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