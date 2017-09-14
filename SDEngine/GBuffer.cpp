#include "GBuffer.h"

GBuffer::GBuffer() : FrontBufferObject() {
	S_FBO = 0;
	AddTextureIndex("worldPosition");
	AddTextureIndex("albedo");
	AddTextureIndex("RMAO");
	AddTextureIndex("normal");
	AddTextureIndex("texCoord");
	AddTextureIndex("translucency");
	AddTextureIndex("HDR");
	AddTextureIndex("finalComp");
}

GBuffer::~GBuffer() {

}



