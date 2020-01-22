#pragma once
#include "Entities/Entity.h"
#include "Rendering/Material.h"

class Grid : public Entity {
public:
	Grid(TString Name, int GridSize = 100, int GridStep = 10) : Entity(Name), S_HalfSize(GridSize / 2), S_StepInterval(GridStep) {
		S_GridMaterial = new Material("Res/Shaders/GridShader");
		S_GridMaterial->SetVec3Parameter("Color", vec3(1.0, 1.0, 1.0));
		S_GridMaterial->SetShaderModel(EShaderModel::UNLIT);
	}
	~Grid() {}

	virtual void Draw(Camera* Camera, bool bCallerProvidesShader = false) override;
private:
	int S_HalfSize;
	int S_StepInterval;
	Material* S_GridMaterial;
};

