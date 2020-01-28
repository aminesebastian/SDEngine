#include "DeserializationStream.h"
#include "Core/Math/MathLibrary.h"

DeserializationStream::DeserializationStream(const ByteBuffer& BufferIn) : Buffer((ByteBuffer&)BufferIn) {
	CurrentIndex = 0;
}
DeserializationStream::DeserializationStream(ByteBuffer& BufferIn) : Buffer(BufferIn) {
	CurrentIndex = 0;
}
void DeserializationStream::DeserializeVec2Array(SArray<vec2>& OutValue) {
	int32 count = 0;
	DeserializeInteger32(count);

	for (int32 i = 0; i < count; i++) {
		vec2 temp;
		DeserializeVec2(temp);
		OutValue.Add(temp);
	}
}
void DeserializationStream::DeserializeVec3Array(SArray<vec3>& OutValue) {
	int32 count = 0;
	DeserializeInteger32(count);

	for (int32 i = 0; i < count; i++) {
		vec3 temp;
		DeserializeVec3(temp);
		OutValue.Add(temp);
	}
}
void DeserializationStream::DeserializeVec4Array(SArray<vec4>& OutValue) {
	int32 count = 0;
	DeserializeInteger32(count);

	for (int32 i = 0; i < count; i++) {
		vec4 temp;
		DeserializeVec4(temp);
		OutValue.Add(temp);
	}
}
void DeserializationStream::DeserializeInteger32Array(SArray<int32>& OutValue) {
	int32 count = 0;
	DeserializeInteger32(count);

	for (int32 i = 0; i < count; i++) {
		int32 temp;
		DeserializeInteger32(temp);
		OutValue.Add(temp);
	}
}
void DeserializationStream::DeserializeUnsignedInteger32Array(SArray<uint32>& OutValue) {
	int32 count = 0;
	DeserializeInteger32(count);

	for (int32 i = 0; i < count; i++) {
		uint32 temp;
		if (i == 34) {
			DeserializeUnsignedInteger32(temp);
		} else {
			DeserializeUnsignedInteger32(temp);
		}
		
		OutValue.Add(temp);
	}
}
void DeserializationStream::DeserializeFloatArray(SArray<float>& OutValue) {
	int32 count = 0;
	DeserializeInteger32(count);

	for (int32 i = 0; i < count; i++) {
		float temp;
		DeserializeFloat(temp);
		OutValue.Add(temp);
	}
}
void DeserializationStream::DeserializeStringArray(SArray<TString>& OutValue) {
	int32 count = 0;
	DeserializeInteger32(count);

	for (int32 i = 0; i < count; i++) {
		TString temp;
		DeserializeString(temp);
		OutValue.Add(temp);
	}
}
void DeserializationStream::DeserializeBoolArray(SArray<bool>& OutValue) {
	int32 count = 0;
	DeserializeInteger32(count);

	for (int32 i = 0; i < count; i++) {
		bool temp;
		DeserializeBool(temp);
		OutValue.Add(temp);
	}
}
void DeserializationStream::DeserializeCharacterArray(SArray<char>& OutValue) {
	int32 count = 0;
	DeserializeInteger32(count);

	for (int32 i = 0; i < count; i++) {
		char temp;
		DeserializeCharacter(temp);
		OutValue.Add(temp);
	}
}

void DeserializationStream::DeserializeVec2(vec2& Value) {
	DeserializeFloat(Value.x);
	DeserializeFloat(Value.y);
}
void DeserializationStream::DeserializeVec3(vec3& Value) {
	DeserializeFloat(Value.x);
	DeserializeFloat(Value.y);
	DeserializeFloat(Value.z);
}
void DeserializationStream::DeserializeVec4(vec4& Value) {
	DeserializeFloat(Value.x);
	DeserializeFloat(Value.y);
	DeserializeFloat(Value.z);
	DeserializeFloat(Value.w);
}
void DeserializationStream::DeserializeInteger32(int32& Value) {
	Value = *(int32*)(&Buffer[CurrentIndex]);
	CurrentIndex += 4;
}
void DeserializationStream::DeserializeUnsignedInteger32(uint32& Value) {
	Value = *(uint32*)(&Buffer[CurrentIndex]);
	CurrentIndex += 4;
}
void DeserializationStream::DeserializeFloat(float& Value) {
	memcpy(&Value, &Buffer[CurrentIndex], sizeof(uint32));
	CurrentIndex += 4;
}
void DeserializationStream::DeserializeString(TString& Value) {
	char* curr = &Buffer[CurrentIndex];
	while ((*curr) != '\0') {
		Value += *curr;
		curr++;
		CurrentIndex++;
	}
}
void DeserializationStream::DeserializeBool(bool& OutValue) {
	OutValue = &Buffer[CurrentIndex];
	CurrentIndex++;
}
void DeserializationStream::DeserializeCharacter(char& OutValue) {
	memcpy(&OutValue, &Buffer[CurrentIndex], sizeof(char));
	CurrentIndex++;
}