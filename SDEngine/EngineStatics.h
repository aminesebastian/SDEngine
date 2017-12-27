#pragma once

class Material;
class Shader;

static Shader* S_LightDebugShader;
static Material* S_DefaultMaterial;
static Shader* S_DefaultGeometryPassShader;
static Shader* S_ShadowShader;


class EngineStatics {
public:
	EngineStatics();
	~EngineStatics();

	static Material* GetDefaultMaterial();
	static Shader* GetLightDebugShader();
	static Shader* GetDefaultGeometryPassShader();
	static Shader* GetShadoowShader();
private:

};

