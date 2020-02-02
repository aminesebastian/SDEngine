#pragma once
#include "Core/Assets/IAssetFactory.h"

class StaticMeshAssetFactory : public IAssetFactory{
public:
	StaticMeshAssetFactory();
	~StaticMeshAssetFactory();
	virtual Asset* CreateNewInstance(TString Name) const override;
	virtual FColor GetAssetColor() const override;
	virtual TString GetAssetType() const override;
};

