#pragma once
#include "Core/DataStructures/ByteBuffer.h"
#include "Core/Utilities/Serialization/SerializationStream.h"
#include "Core/Utilities/Serialization/DeserializationStream.h"

class ISerializeableAsset {
public:
	virtual bool ImportAsset(const TString& FilePath) = 0;
	virtual bool SerializeToBuffer(SerializationStream& Buffer) const = 0;
	virtual bool DeserializeFromBuffer(DeserializationStream& Buffer) = 0;
};