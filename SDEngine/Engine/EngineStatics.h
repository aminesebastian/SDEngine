#pragma once
#include <GLEW/glew.h>
#include <mutex>

class Material;
class Shader;
class RenderTarget;

static Shader* S_LightDebugShader;
static Material* S_DefaultMaterial;
static Shader* S_DefaultGeometryPassShader;
static Shader* S_ShadowShader;
static Shader* S_LineShader;
static Shader* SpriteShader;
static Shader* S_GausBlur7x1Shader;
static Shader* UIShader;

static GLuint SpriteVertexArrayBuffer = 0;
static GLuint quadVBO;

/** Compilation mutex */
static std::mutex S_DefaultShaderCompilationMutex;

class EngineStatics {
public:
	EngineStatics();
	~EngineStatics();

	static void RecompileShaders();

	static Material* GetDefaultMaterial();

	static Shader* GetLightDebugShader();
	static Shader* GetDefaultGeometryPassShader();
	static Shader* GetShadowShader();
	static Shader* GetLineShader();
	static Shader* GetSpriteShader();
	static Shader* GetGausBlur7x1Shader();
	static Shader* GetUIShader();
};

