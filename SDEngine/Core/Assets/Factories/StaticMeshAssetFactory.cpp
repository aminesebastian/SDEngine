#include "StaticMeshAssetFactory.h"
#include "Entities/StaticMesh.h"

StaticMeshAssetFactory::StaticMeshAssetFactory() {

}
StaticMeshAssetFactory::~StaticMeshAssetFactory() {

}
Asset* StaticMeshAssetFactory::CreateNewInstance(TString Name) const {
	return new Asset("StaticMesh", new StaticMesh(Name));
}
FColor StaticMeshAssetFactory::GetAssetColor() const {
	return FColor(0.0f, 0.2f, 1.0f);
}
TString StaticMeshAssetFactory::GetAssetType() const {
	return "StaticMesh";
}