#include "EngineStatics.h"
#include "Material.h"
#include "Shader.h"

EngineStatics::EngineStatics() {}
EngineStatics::~EngineStatics() {}

Shader* EngineStatics::GetLightDebugShader() {
	if(S_LightDebugShader == nullptr) {
		S_LightDebugShader = new Shader("./res/Shaders/LightDebugShader");
	}
	return S_LightDebugShader; 
}
Material* EngineStatics::GetDefaultMaterial() {
	if(S_DefaultMaterial == nullptr) {
		S_DefaultMaterial = new Material(new Shader("./Res/Shaders/DefaultGeometryPassShader"));
		Texture2D* albedoTexture = new Texture2D("./Res/Textures/Checkerboard1K.PNG");
		S_DefaultMaterial->SetTextureParameter("Albedo", albedoTexture);
		S_DefaultMaterial->SetVec3Parameter("Normal", vec3(0.5f, 0.5f, 1.0f));
		S_DefaultMaterial->SetScalarParameter("Roughness", 0.5f);
		S_DefaultMaterial->SetScalarParameter("Metalness", 0.0f);
	}
	return S_DefaultMaterial;
}