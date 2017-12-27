#pragma once
#include "FrameBufferObject.h"
#include "DefferedCompositor.h"
#include "Camera.h"

class PostProcessingLayer {
public:
	PostProcessingLayer();
	~PostProcessingLayer();

	virtual void RenderLayer(DefferedCompositor* Compositor, Camera* Camera, FrameBufferObject* ReadBuffer, FrameBufferObject* OutputBuffer) = 0;
	virtual void RecompileShaders() {}
};

