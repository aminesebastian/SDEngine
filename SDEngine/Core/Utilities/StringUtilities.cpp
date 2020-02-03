#include "StringUtilities.h"
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
	for (std::string val : ret) {
		Output.Add(val);
	}
}