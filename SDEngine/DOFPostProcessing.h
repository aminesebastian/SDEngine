#pragma once
#include "PostProcessingLayer.h"

class DOFLayer : public PostProcessingLayer {
public:
	DOFLayer();
	~DOFLayer();

	virtual void RenderLayer(DefferedCompositor* Compositor, Camera* Camera, FrameBufferObject* ReadBuffer, FrameBufferObject* OutputBuffer) override;
	void RenderXPass(DefferedCompositor* Compositor, Camera* Camera, FrameBufferObject* ReadBuffer, FrameBufferObject* OutputBuffer, bool bFirstPass);
	void RenderYPass(DefferedCompositor* Compositor, Camera* Camera, FrameBufferObject* ReadBuffer, FrameBufferObject* OutputBuffer);
	void BlendOutput(DefferedCompositor* Compositor, Camera* Camera, FrameBufferObject* ReadBuffer, FrameBufferObject* OutputBuffer);
	virtual void RecompileShaders();
private:
	Shader* S_DepthOfFieldShader;

	FrameBufferObject* S_XBlurBuffer;
	FrameBufferObject* S_YBlurBuffer;
};

