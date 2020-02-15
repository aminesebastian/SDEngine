#include "SerializationStream.h"
#include "Core/Utilities/Math/MathLibrary.h"

SerializationStream::SerializationStream(ByteBuffer& BufferIn) : Buffer(BufferIn) {

}
void SerializationStream::SerializeVec2Array(const SArray<vec2>& Array) {
	SerializeInteger32(Array.Count());
	for (vec2 v : Array) {
		SerializeFloat(v.x);
		SerializeFloat(v.y);
	}
}
void SerializationStream::SerializeVec3Array(const SArray<vec3>& Array) {
	SerializeInteger32(Array.Count());
	for (vec3 v : Array) {
		SerializeFloat(v.x);
		SerializeFloat(v.y);
		SerializeFloat(v.z);
	}
}
void SerializationStream::SerializeVec4Array(const SArray<vec4>& Array) {
	SerializeInteger32(Array.Count());
	for (vec4 v : Array) {
		SerializeFloat(v.x);
		SerializeFloat(v.y);
		SerializeFloat(v.z);
		SerializeFloat(v.w);
	}
}
void SerializationStream::SerializeInteger32Array(const SArray<int32>& Array) {
	SerializeInteger32(Array.Count());
	for (int32 i : Array) {
		SerializeInteger32(i);
	}
}
void SerializationStream::SerializeUnsignedInteger32Array(const SArray<uint32>& Array) {
	SerializeInteger32(Array.Count());
	for (uint32 i : Array) {
		SerializeUnsignedInteger32(i);
	}
}
void SerializationStream::SerializeFloatArray(const SArray<float>& Array) {
	SerializeInteger32(Array.Count());
	for (float f : Array) {
		SerializeFloat(f);
	}
}
void SerializationStream::SerializeStringArray(const SArray<TString>& Array) {
	SerializeInteger32(Array.Count());
	for (TString s : Array) {
		SerializeString(s);
	}
}
void SerializationStream::SerializeBoolArray(const SArray<bool>& Array) {
	SerializeInteger32(Array.Count());
	for (bool b : Array) {
		SerializeBool(b);
	}
}
void SerializationStream::SerializeCharacterArray(const SArray<char>& Array) {
	SerializeInteger32(Array.Count());
	for (char c : Array) {
		SerializeCharacter(c);
	}
}
void SerializationStream::SerializeUnsignedCharacterArray(const SArray<unsigned char>& Array) {
	SerializeInteger32(Array.Count());
	for (char c : Array) {
		SerializeUnsignedCharacter(c);
	}
}
void SerializationStream::SerializeVec2(const vec2& Value){
	SerializeFloat(Value.x);
	SerializeFloat(Value.y);
}
void SerializationStream::SerializeVec3(const vec3& Value){
	SerializeFloat(Value.x);
	SerializeFloat(Value.y);
	SerializeFloat(Value.z);
}
void SerializationStream::SerializeVec4(const vec4& Value){
	SerializeFloat(Value.x);
	SerializeFloat(Value.y);
	SerializeFloat(Value.z);
	SerializeFloat(Value.w);
}
void SerializationStream::SerializeInteger32(const int32& Value){
	char* temp = new char[sizeof(int32)];
	memcpy(temp, (char*)&Value, sizeof(int32));
	Buffer.Add(temp[0]);
	Buffer.Add(temp[1]);
	Buffer.Add(temp[2]);
	Buffer.Add(temp[3]);
}
void SerializationStream::SerializeUnsignedInteger32(const uint32& Value){
	char* temp = new char[sizeof(uint32)];
	memcpy(temp, (char*)& Value, sizeof(uint32));
	Buffer.Add(temp[0]);
	Buffer.Add(temp[1]);
	Buffer.Add(temp[2]);
	Buffer.Add(temp[3]);
}
void SerializationStream::SerializeFloat(const float& Value){
	char* b = (char*)&Value;
	Buffer.Add(b[0]);
	Buffer.Add(b[1]);
	Buffer.Add(b[2]);
	Buffer.Add(b[3]);
}
void SerializationStream::SerializeString(const TString& Value){
	for (char c : Value) {
		Buffer.Add(c);
	}
	Buffer.Add('\0');
}
void SerializationStream::SerializeBool(const bool& Value) {
	char* temp = new char[sizeof(bool)];
	memcpy(temp, (char*)& Value, sizeof(bool));
	Buffer.Add(temp[0]);
}
void SerializationStream::SerializeCharacter(const char Value) {
	Buffer.Add(Value);
}
void SerializationStream::SerializeUnsignedCharacter(const unsigned char Value) {
	Buffer.Add(Value);
}