#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Assets/Asset.h"

class IAssetFactory {
public:
	virtual Asset* CreateNewInstance(TString Name) const = 0;
	virtual FColor GetAssetColor() const = 0;
	virtual TString GetAssetType() const = 0;
};