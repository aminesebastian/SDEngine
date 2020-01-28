#pragma once
#include "Core/Assets/IAssetFactory.h"

class MaterialAssetFactory : public IAssetFactory {
public:
	MaterialAssetFactory();
	~MaterialAssetFactory();
	virtual Asset* CreateNewInstance(TString Name) const override;
	virtual FColor GetAssetColor() const override;
	virtual TString GetAssetType() const override;
};

