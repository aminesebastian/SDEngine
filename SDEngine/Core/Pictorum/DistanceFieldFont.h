#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Rendering/Texture2D.h"

struct FDistanceFieldCharacter {
	FDistanceFieldCharacter(char Character, vec2 TextureAtlasDimensions, float UTextureCoord, float VTextureCoord, float Width, float Height, float XOffset, float YOffset, float Advance) :
		Character(Character),
		MinTextureCoords(vec2(UTextureCoord, VTextureCoord) / TextureAtlasDimensions),
		MaxTextureCoord((vec2(UTextureCoord, VTextureCoord) / TextureAtlasDimensions) + vec2(Width, Height) / TextureAtlasDimensions),
		Dimensions(vec2(Width, Height) / TextureAtlasDimensions),
		Offsets(vec2(XOffset, YOffset) / TextureAtlasDimensions),
		Advance((float)Advance / TextureAtlasDimensions.x),
		AspectRatio((float)Width/Height) {
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
	const float AspectRatio;
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

