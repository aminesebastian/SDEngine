#pragma once
#include "Engine/Engine.h"
#include "Core/Assets/Asset.h"
#include "Core/Assets/ISerializeableAsset.h"
#include "Core/Assets/IAssetFactory.h"
#include "Utilities/TypeDefenitions.h"
#include "Utilities/Logger.h"


class AssetManager {

public:
	AssetManager();
	~AssetManager();
	template<class T>
	T* GetAsset(TString AssetPath) {
		if (IsAssetOpen(AssetPath)) {
			return GetAssetFromPool(AssetPath)->Get<T>();
		} else {
			return LoadAsset(AssetPath)->Get<T>();
		}
	}
	bool IsAssetOpen(TString AssetPath) const;
	const IAssetFactory* GetAssetFactoryForType(TString AssetType) const;
	bool RegisterNewFactory(TString AssetType, const IAssetFactory* Factory);
	Asset* GetNewAssetTemplate(TString AssetType) const;

private:
	Asset* LoadAsset(TString AssetPath);
	Asset* GetAssetFromPool(TString AssetPath) const;

	SHashMap<TString, Asset*> AssetCache;
	SHashMap<TString, const IAssetFactory*> FactoryRecords;
};

