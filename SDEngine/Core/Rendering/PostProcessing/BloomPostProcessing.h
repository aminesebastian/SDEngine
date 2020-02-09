#pragma once
#include "PostProcessingLayer.h"

class VariableGausianBlur;

class BloomPostProcessing : public PostProcessingLayer {
public:
	BloomPostProcessing(RenderViewport* OwningViewport);
	~BloomPostProcessing();

	virtual void RenderLayer(const DefferedCompositor* Compositor, const Camera* RenderCamera, GBuffer* ReadBuffer, RenderTarget* PreviousOutput, RenderTarget* OutputBuffer) override;
	virtual void RecompileShaders() override;
	virtual bool PopulatePostProcessingDetailsPanel() override;

	void SetBloomThreshold(float Threshold);
	float GetBloomThreshold();

	void SetBloomSize(float Size);
	float GetBloomSize();

	void SetBloomWeight(float Weight);
	float GetBloomWeight();

private:

	void ClipHDR(const DefferedCompositor* Compositor, RenderTarget* ReadBuffer, RenderTarget* OutputBuffer);
	void BlendOutput(const DefferedCompositor* Compositor, RenderTarget* ReadBuffer, RenderTarget* LitBuffer, RenderTarget* OutputBuffer);

	float BloomThreshold;
	float BloomWeight;
	int BlurPasses;

	Shader* S_ClipHDRShader;
	Shader* S_BlendBloom;

	RenderTarget* ClippedHDRBuffer;
	RenderTarget* BloomOutputBuffer;
	VariableGausianBlur* S_GausBlur;
};

