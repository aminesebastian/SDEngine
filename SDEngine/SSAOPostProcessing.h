#pragma once
#include "PostProcessingLayer.h"
#include "TypeDefenitions.h"

class SSAOPostProcessing : public PostProcessingLayer {
public:
	SSAOPostProcessing();
	~SSAOPostProcessing();

	virtual void RenderLayer(DefferedCompositor* Compositor, Camera* Camera, FrontBufferObject* ReadBuffer, FrontBufferObject* OutputBuffer) override;
	virtual void RecompileShaders();

	void RenderOcclusion(DefferedCompositor* Compositor, Camera* Camera, FrontBufferObject* ReadBuffer, FrontBufferObject* OutputBuffer);
	void Blur(DefferedCompositor* Compositor, Camera* Camera, FrontBufferObject* ReadBuffer, FrontBufferObject* OutputBuffer);
	void GenerateKernel();
	void GenerateNoise();
private:
	Shader* S_SSAOShader;
	FrontBufferObject* S_SSAOBuffer;

	SArray<vec3> S_Noise;
	SArray<vec3> S_Kernel;

	GLuint S_NoiseTexture;

	int S_NoiseSize = 16;
	int S_KernelSize = 64;
};

