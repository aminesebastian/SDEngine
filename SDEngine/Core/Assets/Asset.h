 #pragma once
#include "Core/Assets/ISerializeableAsset.h"
#include "Core/DataStructures/DataStructures.h"
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Engine/Engine.h"
#include "Core/Engine/EngineStatics.h"
#include "Core/Utilities/EngineFunctionLibrary.h"
#include "Core/Utilities/Logger.h"

class AssetMetadata;

/*Should only be created through asset manager*/
class Asset {
public:
	Asset(const TString& AssetType, ISerializeableAsset* AssetPointer);
	~Asset();

	const bool Initialize(const TString& FilePath, const TString& AssetType, ByteBuffer& UncompressedAssetData);
	const bool SerializeForSavingToDisk(ByteBuffer& Buffer);
	const TString GetAssetName() const;
	const TString GetAssetPath() const;
	const TString GetAssetType() const;

	template<typename T>
	T* GetAsset() const {
		return (T*)AssetPointer;
	}
private:
	bool bInitialized;
	TString Name;
	TString FilePath;
	TString AssetType;
	ISerializeableAsset* AssetPointer;
	AssetMetadata* Metadata;
};

