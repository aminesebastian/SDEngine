#pragma once
#include "Core/Assets/ISerializeableAsset.h"
#include "Core/DataStructures/DataStructures.h"
#include "Engine/Engine.h"
#include "Engine/EngineStatics.h"
#include "Utilities/EngineFunctionLibrary.h"
#include "Core/DataTypes/TypeDefenitions.h"
#include "Utilities/Logger.h"


/*Should only be created through asset manager*/
class Asset {
public:
	Asset(TString AssetType, ISerializeableAsset* AssetPointer);
	~Asset();

	bool Initialize(TString FilePath, TString AssetType, ByteBuffer* UncompressedAssetData);
	TString GetAssetName() const;
	TString GetAssetPath() const;
	const SArray<char>& GetRawData() const;

	template<class Type>
	Type* Get() {
		return (Type*)(AssetPointer);
	}
private:
	bool bInitialized;
	TString Name;
	TString FilePath;
	TString AssetType;
	SArray<char> RawData;
	SArray<TString> OldRawData;
	ISerializeableAsset* AssetPointer;
};

