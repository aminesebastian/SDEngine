#include "FileUtilities.h"
#include <fstream>

void FileUtilities::ParseFileIntoLines(const TString& FilePath, SArray<TString>& Output) {
	std::ifstream openedFile(FilePath);
	std::string line;

	while (std::getline(openedFile, line)) {
		Output.Add(line);
	}

	openedFile.close();
}