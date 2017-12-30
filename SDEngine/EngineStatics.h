#pragma once

class Material;
class Shader;

static Shader* S_LightDebugShader;
static Material* S_DefaultMaterial;
static Shader* S_DefaultGeometryPassShader;
static Shader* S_ShadowShader;
static Shader* S_LineShader;

static Shader* S_GausBlur7x1Shader;

class EngineStatics {
public:
	EngineStatics();
	~EngineStatics();

	static Material* GetDefaultMaterial();
	static Shader* GetLightDebugShader();
	static Shader* GetDefaultGeometryPassShader();
	static Shader* GetShadowShader();
	static Shader* GetLineShader();

	static Shader* GetGausBlur7x1Shader();


	//static void BlurTexture(GLuint ReadTexture, GLuint WriteFrameBuffer);
private:

};

