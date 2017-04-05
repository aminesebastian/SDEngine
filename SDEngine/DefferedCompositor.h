#pragma once
#include <GLEW/glew.h>
#include <stdio.h>
#include <cassert>
#include <string>
#include "GBuffer.h"
#include "Shader.h"
#include "Light.h"
#include <vector>

using namespace std;

class DefferedCompositor {
public:
	DefferedCompositor(string LightingShader);
	~DefferedCompositor();

	void CompositeLighting(GBuffer* ReadBuffer, GBuffer* WriteBuffer, vector<Light*> Lights, Camera* camera);
	void CompositePostProcesing(GBuffer* ReadBuffer, GBuffer* WriteBuffer, Camera* Camera);
	void DrawToScreen();
	Shader* GetLightingShader() { return S_LightingShader; }
	Shader* GetPostProcessShader() { return S_PostProcessShader; }
private:

	Shader* S_LightingShader;
	Shader* S_PostProcessShader;
	GLuint quadVAO = 0;
	GLuint quadVBO;

	int S_CurrentBuffer = 0;
};
