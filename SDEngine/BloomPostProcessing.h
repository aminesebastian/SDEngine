#pragma once
#include "PostProcessingLayer.h"

class BloomPostProcessing : public PostProcessingLayer {
public:
	BloomPostProcessing();
	~BloomPostProcessing();

	virtual void RenderLayer(DefferedCompositor* Compositor, Camera* Camera, FrameBufferObject* ReadBuffer, FrameBufferObject* OutputBuffer) override;
	virtual void RecompileShaders();

private:

	void RenderXPass(DefferedCompositor* Compositor, Camera* Camera, FrameBufferObject* ReadBuffer, FrameBufferObject* OutputBuffer, bool bFirstPass);
	void RenderYPass(DefferedCompositor* Compositor, Camera* Camera, FrameBufferObject* ReadBuffer, FrameBufferObject* OutputBuffer);
	void BlendOutput(DefferedCompositor* Compositor, Camera* Camera, FrameBufferObject* ReadBuffer, FrameBufferObject* OutputBuffer);

	Shader* S_BloomShader;
	FrameBufferObject* S_XBloomBuffer;
	FrameBufferObject* S_YBloomBuffer;
};

