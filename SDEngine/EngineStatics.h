#pragma once

class Material;
class Shader;

static Shader* S_LightDebugShader;

class EngineStatics {
public:
	EngineStatics();
	~EngineStatics();

	static Shader* GetLightDebugShader();
private:

};

