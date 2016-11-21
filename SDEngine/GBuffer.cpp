#include <stdio.h>

#include "GBuffer.h"

GBuffer::GBuffer() {
	S_FBO = 0;
	S_DepthBuffer = 0;
}

GBuffer::~GBuffer() {
	if (S_FBO != 0) {
		glDeleteFramebuffers(1, &S_FBO);
	}

	if (S_Textures[0] != 0) {
		glDeleteTextures(GBUFFER_NUM_TEXTURES, S_Textures);
	}

	if (S_Textures != 0) {
		glDeleteTextures(1, &S_DepthBuffer);
	}
}


bool GBuffer::Init(unsigned int WindowWidth, unsigned int WindowHeight) {
	// Create the FBO
	glGenFramebuffers(1, &S_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, S_FBO);

	// Create the gbuffer textures
	glGenTextures(GBUFFER_NUM_TEXTURES, S_Textures);
	glGenTextures(1, &S_DepthBuffer);

	for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {
		glBindTexture(GL_TEXTURE_2D, S_Textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, S_Textures[i], 0);
	}

	// depth
	glBindTexture(GL_TEXTURE_2D, S_DepthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, S_DepthBuffer, 0);

	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3 };

	glDrawBuffers(4, DrawBuffers);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", Status);
		return false;
	}

	// restore default FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	return true;
}


void GBuffer::BindForWriting() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, S_FBO);
}
void GBuffer::BindForReading() {	
	glBindFramebuffer(GL_READ_FRAMEBUFFER, S_FBO);
}
void GBuffer::SetReadBuffer(GBUFFER_TEXTURE_TYPE TextureType) {
	glReadBuffer(GL_COLOR_ATTACHMENT0 + TextureType);
}


