#include "GBuffer.h"

GBuffer::GBuffer() : FrameBufferObject() {
	S_FBO = 0;
	AddTextureIndex(new FFBOTextureEntry("worldPosition", GL_RGBA32F, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT));
	AddTextureIndex(new FFBOTextureEntry("albedo", GL_RGBA32F, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT));
	AddTextureIndex(new FFBOTextureEntry("RMAO", GL_RGBA32F, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT));
	AddTextureIndex(new FFBOTextureEntry("normal", GL_RGBA32F, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT));
	AddTextureIndex(new FFBOTextureEntry("texCoord", GL_RGBA32F, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT));
	AddTextureIndex(new FFBOTextureEntry("translucency", GL_RGBA32F, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT));
	AddTextureIndex(new FFBOTextureEntry("HDR", GL_RGBA32F, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT));
	AddTextureIndex(new FFBOTextureEntry("finalComp", GL_RGBA32F, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT));
}

GBuffer::~GBuffer() {

}



