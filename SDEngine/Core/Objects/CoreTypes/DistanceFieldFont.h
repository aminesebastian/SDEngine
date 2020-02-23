#pragma once
#include "Core/Assets/ISerializeableAsset.h"
#include "Core/Objects/CoreTypes/Texture2D.h"
#include "Core/Objects/EngineObject.h"
#include "Core/DataTypes/TypeDefenitions.h"

struct FDistanceFieldCharacter : public ISerializeableAsset {
	/**
	 * Default constructor for deserialization purposes. Instances of this object should not be
	 * constructed otherwise.
	 */
	FDistanceFieldCharacter() {
		Advance   = 0.0f;
		Character = 0;
	}
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
	bool ImportAsset(const TString& FilePath) override {
		return true;
	}
	bool SerializeToBuffer(SerializationStream& Stream) const override {
		Stream.SerializeCharacter(Character);
		Stream.SerializeVec2(MinTextureCoords);
		Stream.SerializeVec2(MaxTextureCoord);
		Stream.SerializeVec2(Dimensions);
		Stream.SerializeVec2(Offsets);
		Stream.SerializeFloat(Advance);
		Stream.SerializeVec2Array(Verticies);
		Stream.SerializeVec2Array(TextureCoordinates);
		Stream.SerializeInteger32Array(Indices);
		return true;
	}
	bool DeserializeFromBuffer(DeserializationStream& Stream) override {
		Stream.DeserializeCharacter(Character);
		Stream.DeserializeVec2(MinTextureCoords);
		Stream.DeserializeVec2(MaxTextureCoord);
		Stream.DeserializeVec2(Dimensions);
		Stream.DeserializeVec2(Offsets);
		Stream.DeserializeFloat(Advance);
		Stream.DeserializeVec2Array(Verticies);
		Stream.DeserializeVec2Array(TextureCoordinates);
		Stream.DeserializeInteger32Array(Indices);
		return true;
	}
private:
	char Character;
	vec2 MinTextureCoords;
	vec2 MaxTextureCoord;
	vec2 Dimensions;
	vec2 Offsets;
	float Advance;
	SArray<vec2> Verticies;
	SArray<vec2> TextureCoordinates;
	SArray<int32> Indices;
};

class Shader;

class DistanceFieldFont : public EngineObject, public ISerializeableAsset {
public:
	DistanceFieldFont(const TString& DistanceFieldFontName);
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

	bool ImportAsset(const TString& FilePath) override;
	bool SerializeToBuffer(SerializationStream& Stream) const override;
	bool DeserializeFromBuffer(DeserializationStream& Stream) override;
private:
	TString FontName;

	int32 SupportedCharacterCount;
	int32 OfflineFontSize;
	int32 LineHeight;
	int32 Base;
	vec4 Padding;

	SArray<const FDistanceFieldCharacter*> CharacterCache;
	Texture2D* DistanceFieldTexture;
	SArray<TString> SplitLineIntoKeyValuePairs(const TString& Line);
};

