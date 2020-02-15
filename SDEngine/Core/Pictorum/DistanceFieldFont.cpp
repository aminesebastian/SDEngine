#include "DistanceFieldFont.h"
#include "Core/Utilities/StringUtilities.h"
#include "Core/Utilities/FileUtilities.h"
#include "Core/Objects/CoreTypes/Shader.h"
#include "Core/Utilities/Logger.h"
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
	return *CharacterCache[Character];
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
const int32& DistanceFieldFont::GetGeneratedFontSize() const {
	return OfflineFontSize;
}
bool DistanceFieldFont::LoadAndParseFont() {
	// Create and get the texture.
	DistanceFieldTexture = new Texture2D(FontName + "AtlasTexture", ImageFilePath);

	// Preallocate the character cache to cover entirety of ascii extended.
	CharacterCache.Resize(256, nullptr);

	// Split file into array of lines.
	SArray<TString> fileContents;
	FileUtilities::ParseFileIntoLines(FontFilePath, fileContents);

	// Capture the first two lines of metadata.
	SArray<TString> firstLine = SplitLineIntoKeyValuePairs(fileContents[0]);
	SArray<TString> secondLine = SplitLineIntoKeyValuePairs(fileContents[1]);
	SArray<TString> fourthLine = SplitLineIntoKeyValuePairs(fileContents[3]);

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

	int32 charCount = stoi(fourthLine[2]);
	float maxWidth = 0;
	char maxWidthChar = 0;

	// Capture the widest character.
	for (int i = 4; i < charCount; i++) {
		const TString& line = fileContents[i];
		SArray<TString> keyValuePairs = SplitLineIntoKeyValuePairs(line);
		int32 currentWidth = stoi(keyValuePairs[8]);
		if (currentWidth > maxWidth) {
			maxWidth = (float)currentWidth;
			maxWidthChar = (char)stoi(keyValuePairs[2]);
		}
	}

	vec2 atlasDimensions = DistanceFieldTexture->GetDimensions();

	// Parse all the lines.
	for (int i = 4; i < charCount; i++) {
		const TString& line = fileContents[i];
		SArray<TString> keyValuePairs = SplitLineIntoKeyValuePairs(line);

		char character    = stoi(keyValuePairs[2]);
		float minUCoord   = (float)stoi(keyValuePairs[4]) / atlasDimensions.x;
		float minVCoord   = (float)stoi(keyValuePairs[6]) / atlasDimensions.y;
		float width       = (float)stoi(keyValuePairs[8]);
		float height      = (float)stoi(keyValuePairs[10]);
		float xOffset     = (float)stoi(keyValuePairs[12]);
		float yOffset     = (float)stoi(keyValuePairs[14]);
		float xAdvance    = (float)stoi(keyValuePairs[16]);

		FDistanceFieldCharacter* characterEntry = new FDistanceFieldCharacter
		(
			character,
			vec2(minUCoord, minVCoord),
			vec2(minUCoord, minVCoord) + (vec2(width, height) / atlasDimensions),
			vec2(width, height) / maxWidth,
			vec2(xOffset - Padding.x, yOffset - Padding.y) / maxWidth,
			(xAdvance - Padding.x)/maxWidth
		);

		if (character >= 0 && character <= 255) {
			CharacterCache[character] = characterEntry;
		}
	}
	SD_ENGINE_INFO("Loaded Distance Field Font: {0} from File: {1} with maximum width of: {2} on character: {3}.", FontName, FontFilePath, maxWidth, maxWidthChar)
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