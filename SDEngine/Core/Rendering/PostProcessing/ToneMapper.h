#pragma once
#include "PostProcessingLayer.h"

class ToneMapper : public PostProcessingLayer {
public:
	ToneMapper(RenderViewport* OwningViewport);
	~ToneMapper();

	virtual void RenderLayer(const DefferedCompositor* Compositor, const Camera* RenderCamera, GBuffer* ReadBuffer, RenderTarget* PreviousOutput, RenderTarget* OutputBuffer) override;
	virtual void RecompileShaders() override;
private:
	Shader* S_TonemapperShader;
};

