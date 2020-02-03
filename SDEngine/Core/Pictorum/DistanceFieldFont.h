#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Rendering/Texture2D.h"

struct FDistanceFieldCharacter {
	FDistanceFieldCharacter(char Character, vec2 TextureAtlasDimensions, int32 UTextureCoord, int32 VTextureCoord, int32 Width, int32 Height, int32 XOffset, int32 YOffset, int32 Advance) :
		Character(Character),
		TextureCoords(vec2(UTextureCoord, VTextureCoord) / TextureAtlasDimensions),
		Dimensions(vec2(Width, Height) / TextureAtlasDimensions),
		Offsets(vec2(XOffset, YOffset) / TextureAtlasDimensions),
		Advance((float)Advance / TextureAtlasDimensions.x),
		AspectRatio((float)Width/Height){
	}

	const char& GetCharacter() const{
		return Character;
	}
	const float& GetAspectRatio() const {
		return AspectRatio;
	}
	const vec2& GetDimensions() const {
		return Dimensions;
	}
	const vec2& GetOffsets() const {
		return Offsets;
	}
	const vec2& GetTextureCoords() const {
		return TextureCoords;
	}
	const float& GetAdvance() const {
		return Advance;
	}
private:
	const char Character;
	const vec2 TextureCoords;
	const vec2 Dimensions;
	const vec2 Offsets;
	const float AspectRatio;
	const float Advance;
};

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

	const vec2& GetTextureAtlasDimensions();
private:
	TString FontName;
	TString ImageFilePath;
	TString FontFilePath;

	SHashMap<char, const FDistanceFieldCharacter*> CharacterMap;
	Texture2D* DistanceFieldTexture;

	bool LoadAndParseFont();
};

