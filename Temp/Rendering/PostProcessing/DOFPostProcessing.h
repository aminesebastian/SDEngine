#pragma once
#include "PostProcessingLayer.h"

class DOFLayer : public PostProcessingLayer {
public:
	DOFLayer(vec2 FinalOutputDimensions);
	~DOFLayer();

	virtual void RenderLayer(DefferedCompositor* Compositor, Camera* Camera, GBuffer* ReadBuffer, RenderTarget* PreviousOutput, RenderTarget* OutputBuffer) override;
	void RenderXPass(DefferedCompositor* Compositor, Camera* Camera, GBuffer* ReadBuffer, RenderTarget* OutputBuffer, bool bFirstPass);
	void RenderYPass(DefferedCompositor* Compositor, Camera* Camera, GBuffer* ReadBuffer, RenderTarget* OutputBuffer);
	void BlendOutput(DefferedCompositor* Compositor, Camera* Camera, GBuffer* ReadBuffer, RenderTarget* OutputBuffer);
	virtual void RecompileShaders();
private:
	Shader* S_DepthOfFieldShader;

	RenderTarget* S_XBlurBuffer;
	RenderTarget* S_YBlurBuffer;
};

