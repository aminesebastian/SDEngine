#include "StaticMeshComponent.h"
#include "Core/DataStructures/DataStructures.h"
#include "Entities/StaticMesh.h"
#include "Entities/Actor.h"
#include "Rendering/Shader.h";
StaticMeshComponent::StaticMeshComponent(const TString& Name) : Component(Name) {
	Mesh = nullptr;
	test = new Shader("./Res/Shaders/LightDebugShader.frag", "./Res/Shaders/LightDebugShader.vert");
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
			//test->Bind();
			//test->Update(GetWorldTransform(), GetLastFrameTransform(), RenderCamera);
		}
		glBindVertexArray(subMesh->VertexArrayObject);
		glDrawElements(GL_TRIANGLES, subMesh->GetVertexCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}
void StaticMeshComponent::SetStaticMesh(StaticMesh* Mesh) {
	this->Mesh = Mesh;
	this->Mesh->GenerateGPUBuffers();
}
StaticMesh* StaticMeshComponent::GetStaticMesh() {
	return Mesh;
}