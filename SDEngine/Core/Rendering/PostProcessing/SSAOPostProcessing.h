#pragma once
#include "PostProcessingLayer.h"
#include "Core/Rendering/Utilities/VariableGausianBlur.h"
#include "Core/DataTypes/TypeDefenitions.h"

class SSAOPostProcessing : public PostProcessingLayer {
public:
	SSAOPostProcessing(RenderViewport* OwningViewport);
	~SSAOPostProcessing();

	virtual void RenderLayer(const DefferedCompositor* Compositor, const Camera* RenderCamera, GBuffer* ReadBuffer, RenderTarget* PreviousOutput, RenderTarget* OutputBuffer) override;
	virtual void RecompileShaders() override;
	virtual void OnScreenResolutionChanged() override;

	void RenderOcclusion(const DefferedCompositor* Compositor, const Camera* RenderCamera, GBuffer* GBufferIn, RenderTarget* OutputBuffer);
	void Blur(const DefferedCompositor* Compositor, const Camera* RenderCamera, RenderTarget* ReadBuffer, RenderTarget* OutputBuffer);
	void GenerateKernel();
	void GenerateNoise();

	void SetBias(const float& Bias);
	const float& GetBias() const;

	void SetPower(const float& Power);
	const float& GetPower() const;

	void SetRadius(const float& Radius);
	const float& GetRadius() const;

	void SetSampleCount(const int& Samples);
	const int& GetSampleCount() const;

	void SetNoiseSize(const int& Size);
	const int& GetNoiseSize() const;

	void SetBlurRadius(const int& Radius);
	const int& GetBlurRadius() const;

private:
	Shader* SSAOShader;
	RenderTarget* OcclusionBuffer;

	float Bias;
	float Power;
	float Radius;
	int32 NoiseSize;
	int32 KernelSize;
	int32 BlurRadius;
	vec2 NoiseScale;
	GLuint NoiseTexture;
	SArray<vec3> Noise;
	SArray<vec3> SampleKernel;
};

