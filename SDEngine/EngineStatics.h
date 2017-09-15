#pragma once

class Material;
class Shader;

static Shader* S_LightDebugShader;
static Material* S_DefaultMaterial;

class EngineStatics {
public:
	EngineStatics();
	~EngineStatics();

	static Material* GetDefaultMaterial();
	static Shader* GetLightDebugShader();
private:

};

