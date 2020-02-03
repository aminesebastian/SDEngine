#include "DistanceFieldFont.h"
#include "Core/Utilities/StringUtilities.h"
#include "Core/Utilities/FileUtilities.h"
#include "Utilities/Logger.h"
#include <fstream>


DistanceFieldFont::DistanceFieldFont(const TString& DistanceFieldFontName, const TString& FilePath) {
	DistanceFieldTexture = nullptr;
	FontName             = DistanceFieldFontName;
	ImageFilePath        = FilePath + ".png";
	FontFilePath         = FilePath + ".fnt";
	LoadAndParseFont();
}
DistanceFieldFont::~DistanceFieldFont() {
	if (DistanceFieldTexture) {
		delete DistanceFieldTexture;
	}
}
const FDistanceFieldCharacter& DistanceFieldFont::GetDistanceFieldCharacter(const char& Character) const {
	return *CharacterMap.at(Character);
}
const vec2& DistanceFieldFont::GetTextureAtlasDimensions() {
	if (DistanceFieldTexture) {
		return DistanceFieldTexture->GetDimensions();
	}
	return vec2(0.0f, 0.0f);
}
bool DistanceFieldFont::LoadAndParseFont() {
	DistanceFieldTexture = new Texture2D(FontName + "AtlasTexture", ImageFilePath);

	// Split file into array of lines.
	SArray<TString> fileContents;
	FileUtilities::ParseFileIntoLines(FontFilePath, fileContents);

	// Parse all the lines.
	for (int i = 4; i < 101; i++) {
		const TString& line = fileContents[i];

		// Split the line by whitespace.
		SArray<TString> splitLine;
		StringUtilities::SplitStringByWhitespace(line, splitLine);

		// Split the line by equal signs.
		SArray<TString> keyValuePairs;
		for (const TString& pair : splitLine) {
			StringUtilities::SplitString(pair, '=', keyValuePairs);
		}

 		char character = stoi(keyValuePairs[2]);
		int32 uCoord = stoi(keyValuePairs[4]);
		int32 vCoord = stoi(keyValuePairs[6]);
		int32 width = stoi(keyValuePairs[8]);
		int32 height = stoi(keyValuePairs[10]);
		int32 xOffset = stoi(keyValuePairs[12]);
		int32 yOffset = stoi(keyValuePairs[14]);
		int32 xAdvance = stoi(keyValuePairs[16]);

		FDistanceFieldCharacter* characterEntry = new FDistanceFieldCharacter(character, DistanceFieldTexture->GetDimensions(), uCoord, vCoord, width, height, xOffset, yOffset, xAdvance);
		CharacterMap.emplace(character, characterEntry);
	}
	SD_ENGINE_INFO("Loaded Distance Field Font: {0} from File: {1}.", FontName, FontFilePath)
	return true;
}