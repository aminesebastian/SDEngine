#pragma once
#include "Core/Engine/Engine.h"
#include "Core/Assets/Asset.h"
#include "Core/Assets/ISerializeableAsset.h"
#include "Core/Assets/IAssetFactory.h"
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Utilities/Logger.h"


class AssetManager {

public:
	AssetManager();
	~AssetManager();
	template<class T>
	T* FindAsset(const TString& AssetPath) {
		Asset* asset = nullptr;
		if (IsAssetOpen(AssetPath)) {
			asset = GetAssetFromPool(AssetPath);
		} else {
			asset = LoadAsset(AssetPath);
		}
		return asset->GetAsset<T>();
	}
	const bool SaveAssetToDisk(Asset* Target, const TString& AssetPath, const TString& AssetType);
	const bool RegisterNewFactory(const TString& AssetType, const IAssetFactory* Factory);

private:
	Asset* GetNewAssetTemplate(const TString& AssetType) const;
	const IAssetFactory* GetAssetFactoryForType(const TString& AssetType) const;
	const bool IsAssetOpen(const TString& AssetPath) const;

	Asset* LoadAsset(const TString& AssetPath);
	Asset* GetAssetFromPool(const TString& AssetPath) const;

	SHashMap<TString, Asset*> AssetCache;
	SHashMap<TString, const IAssetFactory*> FactoryRecords;
};

