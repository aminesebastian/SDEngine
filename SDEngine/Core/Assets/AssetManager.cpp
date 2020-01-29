#include "AssetManager.h"
#include "Utilities/Logger.h"
#include "Core/Assets/AssetMetadata.h"

AssetManager::AssetManager() {
	SD_ENGINE_INFO("Asset Manager loaded.");
}
AssetManager::~AssetManager() {}
bool AssetManager::SaveAssetToDisk(Asset* Target, const TString& AssetPath, const TString& AssetType) {
	// Capture the asset load start time.
	Uint64 start = SDL_GetPerformanceCounter();
	SD_ENGINE_INFO("Saving Asset: {0} To Disk.");

	// Serialize the asset to a byte buffer.
	ByteBuffer buffer;
	if (!Target->SerializeForSavingToDisk(buffer)) {
		SD_ENGINE_ERROR("An error occured when attempting to serialize asset: {0} to a ByteBuffer.", AssetPath);
		return false;
	}

	// Log serialization time.
	double assetSerializeTime = ((float)(SDL_GetPerformanceCounter() - start)) / SDL_GetPerformanceFrequency();
	SD_ENGINE_INFO("Asset: {0} serialized in: {1:" SEC_FMT "} seconds.", AssetPath, assetSerializeTime);

	// Attempt to write the buffer to a file.
	if (buffer.WriteToCompressedFile(AssetPath)) {
		double assetSaveTime = ((float)(SDL_GetPerformanceCounter() - start)) / SDL_GetPerformanceFrequency();
		SD_ENGINE_INFO("Asset: {0} saved to disk in: {1:" SEC_FMT "} seconds.", AssetPath, assetSaveTime + assetSerializeTime);
		return true;
	} else {
		double assetSaveErrorTime = ((float)(SDL_GetPerformanceCounter() - start)) / SDL_GetPerformanceFrequency();
		SD_ENGINE_INFO("Asset save error for asset: {0} caused a delay of {1:" SEC_FMT "} seconds.", AssetPath, assetSaveErrorTime + assetSerializeTime);
		return false;
	}
}
bool AssetManager::IsAssetOpen(TString AssetPath) const{
	SHashMap<TString, Asset*>::const_iterator it;
	it = AssetCache.find(AssetPath);

	if (it != AssetCache.end()) {
		return true;
	}
	return false;
}
const IAssetFactory* AssetManager::GetAssetFactoryForType(TString AssetType) const{
	SHashMap<TString, const IAssetFactory*>::const_iterator it;
	it = FactoryRecords.find(AssetType);

	if (it != FactoryRecords.end()) {
		return it->second;
	}
	return nullptr;;
}
Asset* AssetManager::GetNewAssetTemplate(TString AssetType) const {
	SHashMap<TString, const IAssetFactory*>::const_iterator it;
	it = FactoryRecords.find(AssetType);

	const IAssetFactory* factory = GetAssetFactoryForType(AssetType);

	if (factory) {
		return factory->CreateNewInstance("TestName");
	}
	return nullptr;
}
bool AssetManager::RegisterNewFactory(TString AssetType, const IAssetFactory* Factory) {
	SHashMap<TString, const IAssetFactory*>::const_iterator it;
	it = FactoryRecords.find(AssetType);

	if (it != FactoryRecords.end()) {
		SD_ENGINE_ERROR("Attempted to register an Asset Factory for type: {0} when one already exists.", AssetType);
		return false;
	}
	FactoryRecords.emplace(AssetType, Factory);
	SD_ENGINE_INFO("Registered Asset Factory for type: {0}.", AssetType);
	return true;
}

Asset* AssetManager::LoadAsset(TString AssetPath) {
	// Capture the asset load start time.
	Uint64 start = SDL_GetPerformanceCounter();

	// Read the file into a byte buffer.
	ByteBuffer buffer;
	if (!buffer.FromCompressedFile(AssetPath)) {
		SD_ENGINE_ERROR("An error occured when attempting to load asset: {0}.", AssetPath);
	}

	// Capture asset read time.
	double assetReadTime = ((float)(SDL_GetPerformanceCounter() - start)) / SDL_GetPerformanceFrequency();
	SD_ENGINE_INFO("Asset: {0} read from asset file in: {1:" SEC_FMT "} seconds.", AssetPath, assetReadTime);

	// Capture the asset metadata.
	DeserializationStream ds(buffer);
	AssetMetadata metadata(ds);
	TString assetType = metadata.GetAssetType();

	// Declare final pointer for loaded asset and attempt to load it.
	// Only set the result pointer to the loaded asset if the asset is initialized successfully.
	Asset* result = GetNewAssetTemplate(assetType);
	if (result) {
		if (!result->Initialize(AssetPath, assetType, buffer)) {
			delete result;
			result = nullptr;
		}
	}

	// Capture the elapsed time.
	double assetLoadTime = ((float)(SDL_GetPerformanceCounter() - start)) / SDL_GetPerformanceFrequency();

	// If the load was successfully, log it as such. Otherwise, log the error.
	if (result) {
		AssetCache.emplace(AssetPath, result);
		SD_ENGINE_INFO("Asset: {0} loaded in: {1:" SEC_FMT "} seconds as: {2}.", AssetPath, assetLoadTime, assetType);
	} else {
		SD_ENGINE_ERROR("Asset load error for asset: {0} caused a delay of: {1:" SEC_FMT "} seconds as: {2}.", AssetPath, assetLoadTime, assetType);
	}

	// Return the result, which is either the loaded asset or a nullptr if the load failed.
	return result;
}
Asset* AssetManager::GetAssetFromPool(TString AssetPath) const{
	SHashMap<TString, Asset*>::const_iterator it;
	it = AssetCache.find(AssetPath);

	if (it != AssetCache.end()) {
		return AssetCache.find(AssetPath)->second;
	}
	return nullptr;
}

