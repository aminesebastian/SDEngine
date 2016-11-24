#pragma once
#include <GLEW/glew.h>
#include <stdio.h>
#include <cassert>
#include "GBuffer.h"
#include "Shader.h"
#include "Light.h"
#include <vector>

using namespace std;

class DefferedCompositor {
public:

	DefferedCompositor();
	~DefferedCompositor();

	void Composite(GBuffer* Buffer, vector<Light*>& Lights, Camera* camera);
	void DrawToScreen();
	Shader& GetLightingShader() { return S_LightingShader; }

private:

	Shader S_LightingShader;
	GLuint quadVAO = 0;
	GLuint quadVBO;
};
