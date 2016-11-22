#pragma once
#include <GLEW/glew.h>
#include <stdio.h>
#include <cassert>
#include "GBuffer.h"
#include "Shader.h"

using namespace std;

class DefferedCompositor {
public:

	DefferedCompositor();
	~DefferedCompositor();

	void Init(unsigned int WindowWidth, unsigned int WindowHeight);
	void Composite(GBuffer* Buffer);
	void DrawToScreen();
	Shader S_LightingShader;
private:


	GLuint quadVAO = 0;
	GLuint quadVBO;
};
