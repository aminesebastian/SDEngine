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