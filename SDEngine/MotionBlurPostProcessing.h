#pragma once
#include "PostProcessingLayer.h"

class MotionBlurPostProcessing : public PostProcessingLayer {
public:
	MotionBlurPostProcessing(vec2 FinalOutputDimensions);
	~MotionBlurPostProcessing();

	virtual void RenderLayer(DefferedCompositor* Compositor, Camera* Camera, GBuffer* ReadBuffer, RenderTarget* PreviousOutput, RenderTarget* OutputBuffer) override;
	virtual void RecompileShaders() override;
	virtual bool PopulatePostProcessingDetailsPanel() override;

	float GetMotionBlurAmount();
	void SetMotionBlurAmount(float Amount);

	int GetMotionBlurSamples();
	void SetMotionBlurSamples(int Samples);

private:
	void ApplyMotionBlur(DefferedCompositor* Compositor, GBuffer* ReadBuffer, RenderTarget* InputBuffer, RenderTarget* OutputBuffer);

	float Amount;
	int Samples;

	Shader* S_MotionBlurShader;
};