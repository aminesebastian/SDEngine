#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Objects/CoreTypes/Texture2D.h"

struct FDistanceFieldCharacter {
	FDistanceFieldCharacter(char Character, vec2 MinTexCoords, vec2 MaxTexCoords, vec2 Dimensions, vec2 Offsets, float Advance) :
		Character(Character),
		MinTextureCoords(MinTexCoords),
		MaxTextureCoord(MaxTexCoords),
		Dimensions(Dimensions),
		Offsets(Offsets),
		Advance(Advance) {
	
		vec2 minTexCoords = GetMinTextureCoords();
		vec2 maxTexCoords = GetMaxTextureCoords();
		vec2 glyphSize = GetDimensions();
		vec2 offsets = GetOffsets();
		vec2 topLeftVert = vec2(offsets.x, -offsets.y);
		vec2 bottomRightVert = topLeftVert + vec2(glyphSize.x, -glyphSize.y);

		TextureCoordinates.Add(vec2(minTexCoords.x, maxTexCoords.y));
		TextureCoordinates.Add(minTexCoords);
		TextureCoordinates.Add(vec2(maxTexCoords.x, minTexCoords.y));
		TextureCoordinates.Add(maxTexCoords);

		Verticies.Add(topLeftVert + vec2(0.0, -glyphSize.y));
		Verticies.Add(topLeftVert);
		Verticies.Add(topLeftVert + vec2(glyphSize.x, 0.0f));
		Verticies.Add(bottomRightVert);

		Indices.Add(2);
		Indices.Add(1);
		Indices.Add(0);
		Indices.Add(3);
		Indices.Add(2);
		Indices.Add(0);
	}

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
	const vec2 GetTotalRequiredSpace() const {
		return Offsets + Dimensions + vec2(Advance, 0.0f);
	}
	const SArray<vec2>& GetVerticies() const {
		return Verticies;
	}
	const SArray<vec2>& GetTextureCoordinates() const {
		return TextureCoordinates;
	}
	const SArray<int32>& GetIndices() const {
		return Indices;
	}
private:
	const char Character;
	const vec2 MinTextureCoords;
	const vec2 MaxTextureCoord;
	const vec2 Dimensions;
	const vec2 Offsets;
	const float Advance;
	SArray<vec2> Verticies;
	SArray<vec2> TextureCoordinates;
	SArray<int32> Indices;
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

	SArray<const FDistanceFieldCharacter*> CharacterCache;
	Texture2D* DistanceFieldTexture;

	bool LoadAndParseFont();
	SArray<TString> SplitLineIntoKeyValuePairs(const TString& Line);
};

