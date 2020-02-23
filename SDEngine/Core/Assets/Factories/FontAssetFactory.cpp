#include "FontAssetFactory.h"
#include "Core/Objects/CoreTypes/DistanceFieldFont.h"

FontAssetFactory::FontAssetFactory() {

}
FontAssetFactory::~FontAssetFactory() {

}
Asset* FontAssetFactory::CreateNewInstance(const TString& Name) const {
	return new Asset("Font", new DistanceFieldFont(Name));
}
FColor FontAssetFactory::GetAssetColor() const {
	return FColor(1.0f, 0.2f, 1.0f);
}
TString FontAssetFactory::GetAssetType() const {
	return "Font";
}