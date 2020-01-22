#include "Engine/EngineStatics.h"
#include "Rendering/Material.h"
#include "Rendering/Shader.h"
#include "Rendering/RenderTarget.h"
#include "Engine/Engine.h"
#include "Rendering/DefferedCompositor.h"

EngineStatics::EngineStatics() {}
EngineStatics::~EngineStatics() {}

void EngineStatics::RecompileShaders() {
	GetLightDebugShader()->RecompileShader();
	GetDefaultGeometryPassShader()->RecompileShader();
	GetShadowShader()->RecompileShader();
	GetLineShader()->RecompileShader();
	GetSpriteShader()->RecompileShader();
}

Shader* EngineStatics::GetLightDebugShader() {
	if(S_LightDebugShader == nullptr) {
		S_LightDebugShader = new Shader("./res/Shaders/LightDebugShader");
	}
	return S_LightDebugShader; 
}
Shader* EngineStatics::GetDefaultGeometryPassShader() {
	if (S_DefaultGeometryPassShader == nullptr) {
		std::lock_guard<std::mutex> guard(S_DefaultShaderCompilationMutex);
		if (S_DefaultGeometryPassShader == nullptr) {
			S_DefaultGeometryPassShader = new Shader("./Res/Shaders/DefaultGeometryPassShader");
		}
	}
	return S_DefaultGeometryPassShader;
}
Shader* EngineStatics::GetShadowShader() {
	if (S_ShadowShader == nullptr) {
		S_ShadowShader = new Shader("./Res/Shaders/ShadowShader");
	}
	return S_ShadowShader;
}
Shader* EngineStatics::GetLineShader() {
	if (S_LineShader == nullptr) {
		S_LineShader = new Shader("./Res/Shaders/LineShader");
	}
	return S_LineShader;
}
Shader* EngineStatics::GetGausBlur7x1Shader() {
	if (S_GausBlur7x1Shader == nullptr) {
		S_GausBlur7x1Shader = new Shader("./Res/Shaders/PostProcessing/GausBlur7x1");
	}
	return S_GausBlur7x1Shader;
}
Shader* EngineStatics::GetSpriteShader() {
	if (SpriteShader == nullptr) {
		SpriteShader = new Shader("./Res/Shaders/EditorSpriteShader");
	}
	return SpriteShader;
}
Material* EngineStatics::GetDefaultMaterial() {
	if(S_DefaultMaterial == nullptr) {
		S_DefaultMaterial = new Material(new Shader("./Res/Shaders/DefaultGeometryPassShader"));
		Texture2D* albedoTexture = new Texture2D("./Res/Textures/Checkerboard1K.PNG");
		S_DefaultMaterial->SetTextureParameter("albedo", albedoTexture);
		Texture2D* normalTexture = new Texture2D("./Res/EmptyNormal.png");
		S_DefaultMaterial->SetTextureParameter("normal", normalTexture);
		Texture2D* RMAO = new Texture2D("./Res/EmptyRMAO.png");
		S_DefaultMaterial->SetTextureParameter("RMAO", RMAO);
	}
	return S_DefaultMaterial;
}
