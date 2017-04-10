#pragma once
#include "Entity.h"
#include "Material.h"

class Grid : public Entity {
public:
	Grid(int GridSize = 100, int GridStep = 10) : S_HalfSize(GridSize / 2), S_StepInterval(GridStep) {
		S_GridMaterial = new Material("Res/Shaders/GridShader");
		S_GridMaterial->SetVec3Parameter("Color", vec3(1.0, 1.0, 1.0));
		S_GridMaterial->SetShaderModel(EShaderModel::UNLIT);
	}
	~Grid() {}

	virtual void Draw(Camera* Camera) override;
private:
	int S_HalfSize;
	int S_StepInterval;
	Material* S_GridMaterial;
};

