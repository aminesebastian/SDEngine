#pragma once
#include "Core/Assets/IAssetFactory.h"

class FontAssetFactory : public IAssetFactory{
public:
	FontAssetFactory();
	~FontAssetFactory();
	virtual Asset* CreateNewInstance(const TString& Name) const override;
	virtual FColor GetAssetColor() const override;
	virtual TString GetAssetType() const override;
};

