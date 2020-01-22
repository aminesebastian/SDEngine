#include "AssetManager.h"



AssetManager::AssetManager() {
	S_AssetCache = *(new SHashMap<TString, SAsset*>());
}
AssetManager::~AssetManager() {

}

SAsset* AssetManager::GetAsset(TString Asset) {
	if(IsAssetOpen(Asset)) {
		return GetAssetFromPool(Asset);
	}else{
		return OpenAsset(Asset);
	}
}

SAsset* AssetManager::OpenAsset(TString Asset) {
	SAsset* temp = new SAsset(Asset);
	S_AssetCache.insert(SPair<TString, SAsset*>(Asset, temp));
	return temp;
}
SAsset* AssetManager::GetAssetFromPool(TString Asset) {
	SHashMap<TString, SAsset*>::const_iterator it;
	it = S_AssetCache.find(Asset);

	if (it != S_AssetCache.end()) {
		return S_AssetCache.find(Asset)->second;
	}
	return nullptr;
}
bool AssetManager::IsAssetOpen(TString Asset) {
	SHashMap<TString, SAsset*>::const_iterator it;
	it = S_AssetCache.find(Asset);

	if (it != S_AssetCache.end()) {
		return true;
	}
	return false;
}