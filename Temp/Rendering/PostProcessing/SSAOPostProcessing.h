#pragma once
#include "PostProcessingLayer.h"
#include "Rendering/Utilities/VariableGausianBlur.h"
#include "Utilities/TypeDefenitions.h"

class SSAOPostProcessing : public PostProcessingLayer {
public:
	SSAOPostProcessing(vec2 FinalOutputDimensions);
	~SSAOPostProcessing();

	virtual void RenderLayer(DefferedCompositor* Compositor, Camera* Camera, GBuffer* GBufferIn, RenderTarget* PreviousOutput, RenderTarget* OutputBuffer) override;
	virtual void RecompileShaders();
	virtual bool PopulatePostProcessingDetailsPanel() override;

	void RenderOcclusion(DefferedCompositor* Compositor, Camera* Camera, GBuffer* GBufferIn, RenderTarget* OutputBuffer);
	void Blur(DefferedCompositor* Compositor, Camera* Camera, RenderTarget* ReadBuffer, RenderTarget* OutputBuffer);
	void GenerateKernel();
	void GenerateNoise();

	void SetBias(float Bias);
	float GetBias();

	void SetPower(float Power);
	float GetPower();

	void SetRadius(float Radius);
	float GetRadius();

	void SetSampleCount(int Samples);
	int GetSampleCount();

	void SetNoiseSize(int Size);
	int GetNoiseSize();

	void SetBlurRadius(int Radius);
	int GetBlurRadius();

private:
	Shader* SSAOShader;
	RenderTarget* OcclusionBuffer;

	float Bias;
	float Power;
	float Radius;
	int S_NoiseSize;
	int S_KernelSize;
	int BlurRadius;
	vec2 NoiseScale;
	GLuint NoiseTexture;
	SArray<vec3> Noise;
	SArray<vec3> SampleKernel;
};

