#pragma once
#include "PostProcessingLayer.h"

class BloomPostProcessing : public PostProcessingLayer {
public:
	BloomPostProcessing();
	~BloomPostProcessing();

	virtual void RenderLayer(DefferedCompositor* Compositor, Camera* Camera, FrontBufferObject* ReadBuffer, FrontBufferObject* OutputBuffer) override;
	virtual void RecompileShaders();

private:

	void RenderXPass(DefferedCompositor* Compositor, Camera* Camera, FrontBufferObject* ReadBuffer, FrontBufferObject* OutputBuffer, bool bFirstPass);
	void RenderYPass(DefferedCompositor* Compositor, Camera* Camera, FrontBufferObject* ReadBuffer, FrontBufferObject* OutputBuffer);
	void BlendOutput(DefferedCompositor* Compositor, Camera* Camera, FrontBufferObject* ReadBuffer, FrontBufferObject* OutputBuffer);

	Shader* S_BloomShader;
	FrontBufferObject* S_XBloomBuffer;
	FrontBufferObject* S_YBloomBuffer;
};

