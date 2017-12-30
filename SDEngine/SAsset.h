#pragma once
#include "Engine.h"
#include "StaticMesh.h"

/*Should only be created through asset manager*/
class SAsset {
public:

	/*File In is closed and pointer is deleted by asset manager*/
	SAsset(TString FilePath);
	~SAsset();

	TString GetAssetName() const { return S_AssetName; }
	TString GetAssetPath() const { return S_FilePath; }
	const SArray<TString>& GetData() { return S_Data; }

	Material* GetAsMaterial();
	StaticMesh* GetAsStaticMesh(TString StaticMeshName);

private:
	TString S_AssetName;
	TString S_FilePath;
	SArray<TString> S_Data;

	TString getFucntionName(TString Line);
	SArray<TString> getParameters(TString Line);
};

