#include "StaticMeshActor.h"
#include "Entities/Components/StaticMeshComponent.h"

StaticMeshActor::StaticMeshActor(const TString& Name) : Actor(Name) {
	RegisterComponent(new StaticMeshComponent("Root"));
}
StaticMeshActor::~StaticMeshActor() {

}	
void StaticMeshActor::SetStaticMesh(StaticMesh* Mesh) {
	if (IsA<StaticMeshComponent>(RootComponent)) {
		Cast<StaticMeshComponent>(RootComponent)->SetStaticMesh(Mesh);
	}
}
StaticMesh* StaticMeshActor::GetMesh() {
	return Cast<StaticMeshComponent>(RootComponent)->GetStaticMesh();
}