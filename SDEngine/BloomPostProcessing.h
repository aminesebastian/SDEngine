#pragma once
#include "PostProcessingLayer.h"

class BloomPostProcessing : public PostProcessingLayer {
public:
	BloomPostProcessing();
	~BloomPostProcessing();

	virtual void RenderLayer(DefferedCompositor* Compositor, Camera* Camera, FrontBufferObject* ReadBuffer, FrontBufferObject* OutputBuffer) override;
	virtual void RecompileShaders();

	void RenderXPass(DefferedCompositor* Compositor, Camera* Camera, FrontBufferObject* ReadBuffer, FrontBufferObject* OutputBuffer);
	void RenderYPass(DefferedCompositor* Compositor, Camera* Camera, FrontBufferObject* ReadBuffer, FrontBufferObject* OutputBuffer);
private:
	Shader* S_BloomShader;
	FrontBufferObject* S_LowResBuffer;
};

