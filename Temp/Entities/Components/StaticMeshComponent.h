#pragma once
#include "Entities/Component.h"

class StaticMesh;
class Shader;

class StaticMeshComponent : public Component {
public:
	StaticMeshComponent(const TString& Name);
	~StaticMeshComponent();

	virtual void DrawAdvanced(Camera* RenderCamera, EDrawType DrawType) override;

	void SetStaticMesh(StaticMesh* Mesh);
	StaticMesh* GetStaticMesh();

private:
	StaticMesh* Mesh;
	Shader* test;
};

