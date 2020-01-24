#include "StaticMeshComponent.h"
#include "Core/DataStructures/DataStructures.h"
#include "Entities/StaticMesh.h"
#include "Entities/Actor.h"

StaticMeshComponent::StaticMeshComponent(const TString& Name) : Component(Name) {
	Mesh = nullptr;
}
StaticMeshComponent::~StaticMeshComponent() {

}
void StaticMeshComponent::DrawAdvanced(Camera* RenderCamera, EDrawType DrawType) {
	if (!Mesh) {
		return;
	}

	for (FSubMesh* subMesh : Mesh->GetSubMeshes()) {
		if (DrawType == SCENE_RENDER) {
			subMesh->SubMeshMaterial->BindMaterial(GetWorldTransform(), GetLastFrameWorldTransform(), RenderCamera);
		}

		glBindVertexArray(subMesh->VertexArrayObject);
		glDrawElements(GL_TRIANGLES, subMesh->GetVertexCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}
void StaticMeshComponent::SetStaticMesh(StaticMesh* Mesh) {
	this->Mesh = Mesh;
}
StaticMesh* StaticMeshComponent::GetStaticMesh() {
	return Mesh;
}