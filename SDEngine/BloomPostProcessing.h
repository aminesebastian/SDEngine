#pragma once
#include "PostProcessingLayer.h"

class VariableGausianBlur;

class BloomPostProcessing : public PostProcessingLayer {
public:
	BloomPostProcessing(vec2 FinalOutputDimensions);
	~BloomPostProcessing();

	virtual void RenderLayer(DefferedCompositor* Compositor, Camera* Camera, GBuffer* ReadBuffer, RenderTarget* PreviousOutput, RenderTarget* OutputBuffer) override;
	virtual void RecompileShaders();
	virtual bool PopulatePostProcessingDetailsPanel() override;

	void SetBloomThreshold(float Threshold);
	float GetBloomThreshold();

	void SetBloomSize(float Size);
	float GetBloomSize();

	void SetBloomWeight(float Weight);
	float GetBloomWeight();

private:

	void ClipHDR(DefferedCompositor* Compositor, RenderTarget* ReadBuffer, RenderTarget* OutputBuffer);
	void BlendOutput(DefferedCompositor* Compositor, RenderTarget* ReadBuffer, RenderTarget* LitBuffer, RenderTarget* OutputBuffer);

	float BloomThreshold;
	float BloomWeight;
	int BlurPasses;

	Shader* S_ClipHDRShader;
	Shader* S_BlendBloom;

	RenderTarget* ClippedHDRBuffer;
	RenderTarget* BloomOutputBuffer;
	VariableGausianBlur* S_GausBlur;
};

