#include "Asset.h"
#include "Core/Assets/AssetManager.h"
#include "Engine/EngineStatics.h"
#include <iostream>
#include <fstream>
#include "Utilities/Logger.h"

Asset::Asset(TString AssetType, ISerializeableAsset* AssetPointer) : AssetType(AssetType), AssetPointer(AssetPointer), bInitialized(false) {}
Asset::~Asset() {}

bool Asset::Initialize(TString FilePath, TString AssetType, ByteBuffer* UncompressedAssetData) {
	if (bInitialized) {
		return true;
	}
	if (!AssetPointer) {
		return false;
	}
	if (!AssetPointer->DeserializeFromBuffer(*UncompressedAssetData)) {
		return false;
	}
	bInitialized = true;
	return true;
}
TString Asset::GetAssetName() const {
	return Name;
}
TString Asset::GetAssetPath() const {
	return FilePath;
}
const SArray<char>& Asset::GetRawData() const {
	return RawData;
}
