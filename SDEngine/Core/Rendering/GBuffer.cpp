#include "GBuffer.h"

GBuffer::GBuffer(vec2 TargetResolution) : RenderTarget(TargetResolution) {
	FrameBuffer = 0;
	AddTextureIndex(new FRenderTargetTextureEntry("worldPosition", GL_RGBA32F, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT));
	AddTextureIndex(new FRenderTargetTextureEntry("albedo", GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT));
	AddTextureIndex(new FRenderTargetTextureEntry("RMAO", GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT));
	AddTextureIndex(new FRenderTargetTextureEntry("normal", GL_RGB8, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_RGB, GL_FLOAT));
	AddTextureIndex(new FRenderTargetTextureEntry("motion", GL_RG8, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_RG, GL_FLOAT));
	AddDepthStencilBuffer();
}

GBuffer::~GBuffer() {

}



