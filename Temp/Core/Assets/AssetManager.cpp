#include "AssetManager.h"


AssetManager::AssetManager() {}
AssetManager::~AssetManager() {}
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
	ByteBuffer* buffer = ByteBuffer::FromCompressedFile(AssetPath);
	if (buffer) {
		Asset* temp = GetNewAssetTemplate("StaticMesh");
		if (temp) {
			temp->Initialize(AssetPath, "StaticMesh", buffer);
		}
		delete buffer;
		return temp;
	}
	delete buffer;
	return nullptr;
}
Asset* AssetManager::GetAssetFromPool(TString AssetPath) const{
	SHashMap<TString, Asset*>::const_iterator it;
	it = AssetCache.find(AssetPath);

	if (it != AssetCache.end()) {
		return AssetCache.find(AssetPath)->second;
	}
	return nullptr;
}

