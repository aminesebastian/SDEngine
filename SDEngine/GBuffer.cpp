#include "GBuffer.h"
#include <iostream>

GBuffer::GBuffer() {
	S_FBO = 0;
	S_GBufferTextureNames.push_back("worldPosition");
	S_GBufferTextureNames.push_back("albedo");
	S_GBufferTextureNames.push_back("RMAO");
	S_GBufferTextureNames.push_back("normal");
	S_GBufferTextureNames.push_back("texCoord");
	S_GBufferTextureNames.push_back("translucency");
	S_GBufferTextureNames.push_back("HDR");
	S_GBufferTextureNames.push_back("finalComp");
}

GBuffer::~GBuffer() {
	if (S_FBO != 0) {
		glDeleteFramebuffers(1, &S_FBO);
	}
	//if (S_Textures[0] != 0) {
	//	glDeleteTextures(GBUFFER_NUM_TEXTURES, S_Textures);
	//}
}

bool GBuffer::Init(unsigned int WindowWidth, unsigned int WindowHeight) {
	glDeleteFramebuffersEXT(1, &S_FBO);
	glDeleteTextures(GBUFFER_NUM_TEXTURES, S_Textures);

	glGenFramebuffers(1, &S_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, S_FBO);

	//Optimize Bit Depths Later
	for (int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {
		glGenTextures(1, &S_Textures[i]);
		glBindTexture(GL_TEXTURE_2D, S_Textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WindowWidth, WindowHeight, 0, GL_RGBA, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, S_Textures[i], 0);
	}

	GLuint attachments[8] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3,
		GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7};

	glDrawBuffers(8, attachments);

	glGenRenderbuffers(1, &S_DepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, S_DepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, S_DepthBuffer);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "Framebuffer not complete!" << std::endl;
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}


void GBuffer::BindForWriting() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, S_FBO);
}
void GBuffer::BindForReading() {	
	glBindFramebuffer(GL_READ_FRAMEBUFFER, S_FBO);
}

/**********************************************************************************************//**
 * @fn	void GBuffer::SetReadBuffer(GBUFFER_TEXTURE_TYPE TextureType)
 *
 * @brief	Sets read buffer.
 *
 * @author	Amine
 * @date	2/20/2017
 *
 * @param	TextureType	Type of the texture.
 **************************************************************************************************/

void GBuffer::SetReadBuffer(GBUFFER_TEXTURE_TYPE TextureType) {
	glReadBuffer(GL_COLOR_ATTACHMENT0 + TextureType);
}


