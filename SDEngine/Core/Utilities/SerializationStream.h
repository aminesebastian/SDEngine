#pragma once
#include "Core/DataStructures/ByteBuffer.h"
#include "Core/DataTypes/TypeDefenitions.h"

class SerializationStream {
public:
	SerializationStream(ByteBuffer& BufferIn);

	void SerializeVec2Array(const SArray<vec2>& Array);
	void SerializeVec3Array(const SArray<vec3>& Array);
	void SerializeVec4Array(const SArray<vec4>& Array);
	void SerializeInteger32Array(const SArray<int32>& Array);
	void SerializeUnsignedInteger32Array(const SArray<uint32>& Array);
	void SerializeFloatArray(const SArray<float>& Array);
	void SerializeStringArray(const SArray<TString>& Array);
	void SerializeBoolArray(const SArray<bool>& Array);
	void SerializeCharacterArray(const SArray<char>& Array);

	void SerializeVec2(const vec2& Value);
	void SerializeVec3(const vec3& Value);
	void SerializeVec4(const vec4& Value);
	void SerializeInteger32(const int32& Value);
	void SerializeUnsignedInteger32(const uint32& Value);
	void SerializeFloat(const float& Value);
	void SerializeString(const TString& Value);
	void SerializeBool(const bool& Value);
	void SerializeCharacter(const char Value);
private:
	ByteBuffer& Buffer;
};

