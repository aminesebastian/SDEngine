#pragma once
#include "Core/DataStructures/Array.h"
#include "Core/DataTypes/TypeDefenitions.h"
#include <sstream>
#include <algorithm> 
#include <cctype>
#include <locale>

class StringUtilities {
public:
	static void SplitString(const TString& String, const char& Delimeter, SArray<TString>& Output);
	static void SplitStringByWhitespace(const TString& String, SArray<TString>& Output);

	static const TString LeftTrim(const TString& String);
	static const TString RightTrim(const TString& String);
	static const TString Trim(const TString& String);

	template <typename T>
	static TString ToStringWithPrecision(const T& Value, const int32& Precision) {
		std::ostringstream out;
		out.precision(Precision);
		out << std::fixed << Value;
		return out.str();
	}
};
