#pragma once
#include "Engine/Engine.h"
#include "SAsset.h"
#include "Utilities/TypeDefenitions.h"


class AssetManager {

public:
	AssetManager();
	~AssetManager();

	SAsset* GetAsset(TString Asset);
	bool IsAssetOpen(TString Asset);

private:
	SAsset* GetAssetFromPool(TString Asset);
	SAsset* OpenAsset(TString Asset);

	SHashMap<TString, SAsset*> S_AssetCache;
};
