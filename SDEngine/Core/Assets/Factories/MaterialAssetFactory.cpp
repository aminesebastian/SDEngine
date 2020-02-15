#include "MaterialAssetFactory.h"
#include "Core/Objects/CoreTypes/Material.h"

MaterialAssetFactory::MaterialAssetFactory() {

}
MaterialAssetFactory::~MaterialAssetFactory() {

}
Asset* MaterialAssetFactory::CreateNewInstance(TString Name) const {
	return new Asset("Material", new Material(Name));
}
FColor MaterialAssetFactory::GetAssetColor() const {
	return FColor(0.0f, 1.0f, 0.1f);
}
TString MaterialAssetFactory::GetAssetType() const {
	return "Material";
}