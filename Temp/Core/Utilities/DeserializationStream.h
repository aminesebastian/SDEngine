#pragma once
#include "Core/DataStructures/ByteBuffer.h"
#include "Utilities/TypeDefenitions.h"

class DeserializationStream {
public:
	DeserializationStream(const ByteBuffer& BufferIn);
	DeserializationStream(ByteBuffer& BufferIn);

	void DeserializeVec2Array(SArray<vec2>& OutValue);
	void DeserializeVec3Array(SArray<vec3>& OutValue);
	void DeserializeVec4Array(SArray<vec4>& OutValue);
	void DeserializeInteger32Array(SArray<int32>& OutValue);
	void DeserializeUnsignedInteger32Array(SArray<uint32>& OutValue);
	void DeserializeFloatArray(SArray<float>& OutValue);
	void DeserializeStringArray(SArray<TString>& OutValue);
	void DeserializeBoolArray(SArray<bool>& OutValue);

	void DeserializeVec2(vec2& OutValue);
	void DeserializeVec3(vec3& OutValue);
	void DeserializeVec4(vec4& OutValue);
	void DeserializeInteger32(int32& OutValue);
	void DeserializeUnsignedInteger32(uint32& OutValue);
	void DeserializeFloat(float& OutValue);
	void DeserializeString(TString& OutValue);
	void DeserializeBool(bool& OutValue);
private:
	ByteBuffer& Buffer;
	int CurrentIndex;
};

