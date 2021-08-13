#include "StaticMeshAssetFactory.h"
#include "Core/Objects/CoreTypes/StaticMesh.h"

StaticMeshAssetFactory::StaticMeshAssetFactory() {

}
StaticMeshAssetFactory::~StaticMeshAssetFactory() {

}
Asset* StaticMeshAssetFactory::CreateNewInstance(const TString& Name) const {
	return new Asset("StaticMesh", new StaticMesh(Name));
}
FColor StaticMeshAssetFactory::GetAssetColor() const {
	return FColor(0.0f, 0.2f, 1.0f);
}
TString StaticMeshAssetFactory::GetAssetType() const {
	return "StaticMesh";
}