#pragma once
#include "FrontBufferObject.h"
#include "DefferedCompositor.h"
#include "Camera.h"

class PostProcessingLayer {
public:
	PostProcessingLayer();
	~PostProcessingLayer();

	virtual void RenderLayer(DefferedCompositor* Compositor, Camera* Camera, FrontBufferObject* ReadBuffer, FrontBufferObject* OutputBuffer) = 0;
	virtual void RecompileShaders() {}
};

