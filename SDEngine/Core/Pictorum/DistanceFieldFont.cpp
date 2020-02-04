#include "DistanceFieldFont.h"
#include "Core/Utilities/StringUtilities.h"
#include "Core/Utilities/FileUtilities.h"
#include "Rendering/Shader.h"
#include "Utilities/Logger.h"
#include <fstream>


DistanceFieldFont::DistanceFieldFont(const TString& DistanceFieldFontName, const TString& FilePath) {
	DistanceFieldTexture = nullptr;
	FontName = DistanceFieldFontName;
	ImageFilePath = FilePath + ".png";
	FontFilePath = FilePath + ".fnt";
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
const vec2& DistanceFieldFont::GetTextureAtlasDimensions() const {
	if (DistanceFieldTexture) {
		return DistanceFieldTexture->GetDimensions();
	}
	return vec2(0.0f, 0.0f);
}
void DistanceFieldFont::BindAtlas(Shader* ShaderIn, TString ParameterName, int32 TextureOffset) const {
	DistanceFieldTexture->Bind(ParameterName, ShaderIn, TextureOffset);
}
const int32& DistanceFieldFont::GetGeneratedFontSize() const{
	return OfflineFontSize;
}
bool DistanceFieldFont::LoadAndParseFont() {
	DistanceFieldTexture = new Texture2D(FontName + "AtlasTexture", ImageFilePath);

	// Split file into array of lines.
	SArray<TString> fileContents;
	FileUtilities::ParseFileIntoLines(FontFilePath, fileContents);

	// Capture the first two lines of metadata.
	SArray<TString> firstLine = SplitLineIntoKeyValuePairs(fileContents[0]);
	SArray<TString> secondLine = SplitLineIntoKeyValuePairs(fileContents[1]);

	// Capture the font size the atlas was generated at.
	OfflineFontSize = stoi(firstLine[4]);

	// Capture the padding values.
	TString padding = firstLine[20];
	SArray<TString> paddingValues;
	StringUtilities::SplitString(padding, ',', paddingValues);
	Padding = vec4(stoi(paddingValues[0]), stoi(paddingValues[1]), stoi(paddingValues[2]), stoi(paddingValues[3]));

	// Capture the line height and base;
	LineHeight = stoi(secondLine[2]);
	Base = stoi(secondLine[4]);

	// Parse all the lines.
	for (int i = 4; i < 101; i++) {
		const TString& line = fileContents[i];
		SArray<TString> keyValuePairs = SplitLineIntoKeyValuePairs(line);

		char character = stoi(keyValuePairs[2]);
		int32 uCoord = stoi(keyValuePairs[4]);
		int32 vCoord = stoi(keyValuePairs[6]);
		int32 width = stoi(keyValuePairs[8]);
		int32 height = stoi(keyValuePairs[10]);
		int32 xOffset = stoi(keyValuePairs[12]);
		int32 yOffset = stoi(keyValuePairs[14]);
		int32 xAdvance = stoi(keyValuePairs[16]);

		FDistanceFieldCharacter* characterEntry = new FDistanceFieldCharacter
		(
			character, 
			DistanceFieldTexture->GetDimensions(), 
			(float)uCoord, 
			(float)vCoord, 
			(float)width, 
			(float)height, 
			(float)xOffset - Padding.x,  
			(float)yOffset - Padding.y,  
			(float)xAdvance - Padding.x
		);
		CharacterMap.emplace(character, characterEntry);
	}
	SD_ENGINE_INFO("Loaded Distance Field Font: {0} from File: {1}.", FontName, FontFilePath)
		return true;
}
SArray<TString> DistanceFieldFont::SplitLineIntoKeyValuePairs(const TString& Line) {
	SArray<TString> splitByWhitespace;
	StringUtilities::SplitStringByWhitespace(Line, splitByWhitespace);

	SArray<TString> splitByEquals;
	for (const TString& pair : splitByWhitespace) {
		StringUtilities::SplitString(pair, '=', splitByEquals);
	}
	return splitByEquals;
}