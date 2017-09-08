#include "AssetManager.h"



AssetManager::AssetManager() {
	S_AssetPool = *(new SArray<SAsset*>());
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
	S_AssetPool.push_back(temp);
	return temp;
}
SAsset* AssetManager::GetAssetFromPool(TString Asset) {
	for (int i = 0; i < S_AssetPool.size(); i++) {
		if (S_AssetPool[i]->GetAssetPath() == Asset) {
			return S_AssetPool[i];
		}
	}
	return nullptr;
}
bool AssetManager::IsAssetOpen(TString Asset) {
	for(int i=0; i<S_AssetPool.size(); i++) {
		if(S_AssetPool[i]->GetAssetPath() == Asset) {
			return true;
		}
	}
	return false;
}