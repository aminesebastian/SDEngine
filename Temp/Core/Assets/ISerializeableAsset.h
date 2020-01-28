#pragma once
#include "Core/DataStructures/ByteBuffer.h"

class ISerializeableAsset {
public:
	virtual bool SerializeToBuffer(ByteBuffer& Buffer) const = 0;
	virtual bool DeserializeFromBuffer(const ByteBuffer& Buffer) = 0;
};