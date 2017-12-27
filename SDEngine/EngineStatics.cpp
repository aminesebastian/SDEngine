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
Shader* EngineStatics::GetDefaultGeometryPassShader() {
	if (S_DefaultGeometryPassShader == nullptr) {
		S_DefaultGeometryPassShader = new Shader("./Res/Shaders/DefaultGeometryPassShader");
	}
	return S_DefaultGeometryPassShader;
}
Shader* EngineStatics::GetShadoowShader() {
	if (S_ShadowShader == nullptr) {
		S_ShadowShader = new Shader("./Res/Shaders/ShadowShader");
	}
	return S_ShadowShader;
}

Material* EngineStatics::GetDefaultMaterial() {
	if(S_DefaultMaterial == nullptr) {
		S_DefaultMaterial = new Material(new Shader("./Res/Shaders/DefaultGeometryPassShader"));
		Texture2D* albedoTexture = new Texture2D("./Res/Textures/Checkerboard1K.PNG");
		S_DefaultMaterial->SetTextureParameter("Albedo", albedoTexture);
		Texture2D* normalTexture = new Texture2D("./Res/NormalTest.jpg");
		S_DefaultMaterial->SetTextureParameter("normal", normalTexture);
		S_DefaultMaterial->SetScalarParameter("Roughness", 0.5f);
		S_DefaultMaterial->SetScalarParameter("Metalness", 0.0f);
	}
	return S_DefaultMaterial;
}