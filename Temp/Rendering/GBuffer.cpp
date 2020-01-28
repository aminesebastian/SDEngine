#include "GBuffer.h"

GBuffer::GBuffer(vec2 TargetResolution) : RenderTarget(TargetResolution) {
	FrameBuffer = 0;
	AddTextureIndex(new FRenderTargetTextureEntry("worldPosition", GL_RGBA32F, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT));
	AddTextureIndex(new FRenderTargetTextureEntry("albedo", GL_RGBA32F, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT));
	AddTextureIndex(new FRenderTargetTextureEntry("RMAO", GL_RGBA32F, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT));
	AddTextureIndex(new FRenderTargetTextureEntry("normal", GL_RGBA32F, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT));
	AddTextureIndex(new FRenderTargetTextureEntry("texCoord", GL_RGBA32F, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT));
	AddTextureIndex(new FRenderTargetTextureEntry("translucency", GL_RGBA32F, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT));
	AddTextureIndex(new FRenderTargetTextureEntry("motion", GL_RGBA32F, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT));
	AddDepthStencilBuffer();
}

GBuffer::~GBuffer() {

}



