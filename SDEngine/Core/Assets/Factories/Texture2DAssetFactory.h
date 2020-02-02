#pragma once
#include "Core/Assets/IAssetFactory.h"

class Texture2DAssetFactory : public IAssetFactory{
public:
	Texture2DAssetFactory();
	~Texture2DAssetFactory();
	virtual Asset* CreateNewInstance(TString Name) const override;
	virtual FColor GetAssetColor() const override;
	virtual TString GetAssetType() const override;
};

