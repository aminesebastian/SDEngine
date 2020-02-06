#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Rendering/Texture2D.h"

struct FDistanceFieldCharacter {
	FDistanceFieldCharacter(char Character, vec2 MinTexCoords, vec2 MaxTexCoords, vec2 Dimensions, vec2 Offsets, float Advance) :
		Character(Character),
		MinTextureCoords(MinTexCoords),
		MaxTextureCoord(MaxTexCoords),
		Dimensions(Dimensions),
		Offsets(Offsets),
		Advance(Advance) {}

	const char& GetCharacter() const {
		return Character;
	}
	const vec2& GetDimensions() const {
		return Dimensions;
	}
	const vec2& GetOffsets() const {
		return Offsets;
	}
	const vec2& GetMinTextureCoords() const {
		return MinTextureCoords;
	}
	const vec2& GetMaxTextureCoords() const {
		return MaxTextureCoord;
	}
	const float& GetAdvance() const {
		return Advance;
	}
private:
	const char Character;
	const vec2 MinTextureCoords;
	const vec2 MaxTextureCoord;
	const vec2 Dimensions;
	const vec2 Offsets;
	const float Advance;
};

class Shader;

class DistanceFieldFont {
public:
	DistanceFieldFont(const TString& DistanceFieldFontName, const TString& FilePath);
	~DistanceFieldFont();

	/**
	 * Gets distance field character data for the provided character.
	 *
	 * @param 	{const char&}	Character	The character.
	 *
	 * @returns	{const FDistanceFieldCharacter*}	Null if it fails, else the distance field
	 * 												character.
	 */
	const FDistanceFieldCharacter& GetDistanceFieldCharacter(const char& Character) const;
	const vec2& GetTextureAtlasDimensions() const;
	void BindAtlas(Shader* ShaderIn, TString ParameterName, int32 TextureOffset) const;

	const int32& GetGeneratedFontSize() const;
private:
	TString FontName;
	TString ImageFilePath;
	TString FontFilePath;

	int32 OfflineFontSize;
	int32 LineHeight;
	int32 Base;
	vec4 Padding;

	SHashMap<char, const FDistanceFieldCharacter*> CharacterMap;
	Texture2D* DistanceFieldTexture;

	bool LoadAndParseFont();
	SArray<TString> SplitLineIntoKeyValuePairs(const TString& Line);
};

