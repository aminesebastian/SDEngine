#pragma once
#include "PostProcessingLayer.h"

class ToneMapper : public PostProcessingLayer {
public:
	ToneMapper(vec2 FinalOutputDimensions);
	~ToneMapper();

	virtual void RenderLayer(DefferedCompositor* Compositor, Camera* Camera, GBuffer* ReadBuffer, RenderTarget* PreviousOutput, RenderTarget* OutputBuffer) override;
	virtual void RecompileShaders();
private:
	Shader* S_TonemapperShader;
};

