#include "Texture2DAssetFactory.h"
#include "Core/Objects/CoreTypes/Texture2D.h"

Texture2DAssetFactory::Texture2DAssetFactory() {

}
Texture2DAssetFactory::~Texture2DAssetFactory() {

}
Asset* Texture2DAssetFactory::CreateNewInstance(const TString& Name) const {
	return new Asset("Texture2D", new Texture2D(Name));
}
FColor Texture2DAssetFactory::GetAssetColor() const {
	return FColor(0.0f, 0.2f, 1.0f);
}
TString Texture2DAssetFactory::GetAssetType() const {
	return "Texture2D";
}