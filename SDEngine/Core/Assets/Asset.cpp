#include "Asset.h"
#include "Core/Assets/AssetManager.h"
#include "Core/Assets/AssetMetadata.h"
#include "Core/Engine/EngineStatics.h"
#include <iostream>
#include <fstream>
#include "Core/Utilities/Logger.h"

Asset::Asset(TString AssetType, ISerializeableAsset* AssetPointer) : AssetType(AssetType), AssetPointer(AssetPointer), bInitialized(false) {
	Metadata = nullptr;
}
Asset::~Asset() {}

bool Asset::Initialize(TString FilePath, TString AssetType, ByteBuffer& UncompressedAssetData) {
	this->FilePath = FilePath;
	this->AssetType = AssetType;

	if (bInitialized) {
		return true;
	}
	if (!AssetPointer) {
		return false;
	}

	DeserializationStream stream(UncompressedAssetData);
	Metadata = new AssetMetadata(stream);
	if (!AssetPointer->DeserializeFromBuffer(stream)) {
		return false;
	}
	bInitialized = true;
	return true;
}
bool Asset::SerializeForSavingToDisk(ByteBuffer& Buffer) {
	if (!bInitialized) {
		return nullptr;
	}
	if (!AssetPointer) {
		return false;
	}

	SerializationStream stream(Buffer);
	Metadata = new AssetMetadata(GetAssetType(), 1, stream);
	if (!AssetPointer->SerializeToBuffer(stream)) {
		return false;
	}
	return true;
}
TString Asset::GetAssetName() const {
	return Name;
}
TString Asset::GetAssetPath() const {
	return FilePath;
}
TString Asset::GetAssetType() const {
	return AssetType;
}
const SArray<char>& Asset::GetRawData() const {
	return RawData;
}
