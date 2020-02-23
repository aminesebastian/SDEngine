#include "Asset.h"
#include "Core/Assets/AssetManager.h"
#include "Core/Assets/AssetMetadata.h"
#include "Core/Engine/EngineStatics.h"
#include <iostream>
#include <fstream>
#include "Core/Utilities/Logger.h"

Asset::Asset(const TString& AssetType, ISerializeableAsset* AssetPointer) : AssetType(AssetType), AssetPointer(AssetPointer), bInitialized(false) {
	Name     = "";
	FilePath = "";
	Metadata = nullptr;
}
Asset::~Asset() {}

const bool Asset::Initialize(const TString& FilePath, const TString& AssetType, ByteBuffer& UncompressedAssetData) {
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
const bool Asset::SerializeForSavingToDisk(ByteBuffer& Buffer) {
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
const TString Asset::GetAssetName() const {
	return Name;
}
const TString Asset::GetAssetPath() const {
	return FilePath;
}
const TString Asset::GetAssetType() const {
	return AssetType;
}
