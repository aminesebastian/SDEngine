#pragma once
#include "PostProcessingLayer.h"

class ToneMapper : public PostProcessingLayer {
public:
	ToneMapper();
	~ToneMapper();

	virtual void RenderLayer(DefferedCompositor* Compositor, Camera* Camera, FrameBufferObject* ReadBuffer, FrameBufferObject* OutputBuffer) override;
	virtual void RecompileShaders();
private:
	Shader* S_TonemapperShader;
};

