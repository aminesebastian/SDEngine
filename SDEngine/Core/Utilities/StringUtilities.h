#pragma once
#include "Core/DataStructures/Array.h"
#include "Core/DataTypes/TypeDefenitions.h"

class StringUtilities {
public:
	static void SplitString(const TString& String, const char& Delimeter, SArray<TString>& Output);
	static void SplitStringByWhitespace(const TString& String, SArray<TString>& Output);
};


