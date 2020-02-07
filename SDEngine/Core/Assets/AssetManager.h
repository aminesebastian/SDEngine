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
	T* GetAsset(TString AssetPath) {
		if (IsAssetOpen(AssetPath)) {
			return GetAssetFromPool(AssetPath)->Get<T>();
		} else {
			return LoadAsset(AssetPath)->Get<T>();
		}
	}
	bool SaveAssetToDisk(Asset* Target, const TString& AssetPath, const TString& AssetType);
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

