#pragma once
#include "Core/DataStructures/Array.h"
#include "Core/DataTypes/TypeDefenitions.h"

class FileUtilities {
public:
	static void ParseFileIntoLines(const TString& FilePath, SArray<TString>& Output);
};

