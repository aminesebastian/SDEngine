#pragma once
#include "PostProcessingLayer.h"

class MotionBlurPostProcessing : public PostProcessingLayer {
public:
	MotionBlurPostProcessing(RenderViewport* OwningViewport);
	~MotionBlurPostProcessing();

	virtual void RenderLayer(const DefferedCompositor* Compositor, const Camera* RenderCamera, GBuffer* ReadBuffer, RenderTarget* PreviousOutput, RenderTarget* OutputBuffer) override;
	virtual void RecompileShaders() override;
	virtual bool PopulatePostProcessingDetailsPanel() override;

	float GetMotionBlurAmount();
	void SetMotionBlurAmount(float Amount);

	int GetMotionBlurSamples();
	void SetMotionBlurSamples(int Samples);

private:
	void ApplyMotionBlur(const DefferedCompositor* Compositor, GBuffer* ReadBuffer, RenderTarget* InputBuffer, RenderTarget* OutputBuffer);

	float Amount;
	int Samples;

	Shader* S_MotionBlurShader;
};