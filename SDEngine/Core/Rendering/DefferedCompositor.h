#pragma once
#include <GLEW/glew.h>
#include <stdio.h>
#include <cassert>
#include <string>
#include "Core/Rendering/GBuffer.h"
#include "Core/Objects/CoreTypes/Shader.h"
#include "Core/Objects/Entities/Light.h"
#include "Core/DataStructures/DataStructures.h"

using namespace std;

class DefferedCompositor {
public:
	DefferedCompositor(string LightingShader);
	~DefferedCompositor();

	void CompositeLighting(GBuffer* GBufferIn, RenderTarget* OutputBuffer, SArray<Light*> Lights, Camera* camera);
	void CompositePostProcesing(GBuffer* ReadBuffer, GBuffer* WriteBuffer, Camera* Camera, int PostProcessingIndex);
	void OutputToScreen(RenderTarget* ReadBuffer);

	void RecompileShaders();

	void DrawScreenQuad();
	Shader* GetLightingShader() { return S_LightingShader; }
	Shader* GetPostProcessShader(int Index) { return S_PostProcessingShaders[Index]; }
	int GetPostProcessShaderCount() { return (int)S_PostProcessingShaders.size(); }
private:

	Shader* S_LightingShader;
	Shader* S_FinalOutputShader;
	vector<Shader*> S_PostProcessingShaders;
	GLuint quadVAO = 0;
	GLuint quadVBO;

	int S_CurrentBuffer = 0;
};
