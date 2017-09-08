#pragma once
#include "Engine.h"
#include "SAsset.h"


class AssetManager {

public:
	AssetManager();
	~AssetManager();

	SAsset* GetAsset(TString Asset);
	bool IsAssetOpen(TString Asset);

private:
	SAsset* GetAssetFromPool(TString Asset);
	SAsset* OpenAsset(TString Asset);

	SArray<SAsset*> S_AssetPool;
};

