#include "FrontBufferObject.h"
#include <iostream>
#include "Shader.h"

FrontBufferObject::FrontBufferObject() {
	S_NumTextures = 0;
	S_FBO = 0;
}
FrontBufferObject::~FrontBufferObject() {
	if (S_FBO != 0) {
		glDeleteFramebuffers(1, &S_FBO);
	}
	if (S_Textures[0] != 0) {
		glDeleteTextures(S_NumTextures, &S_Textures[0]);
	}
}

void FrontBufferObject::AddTextureIndex(TString TextureName) {
	if(S_NumTextures < 8) {
		S_TextureNames.push_back(TextureName);
		S_NumTextures++;
	}
}
bool FrontBufferObject::Init(unsigned int WindowWidth, unsigned int WindowHeight) {
	glDeleteFramebuffers(1, &S_FBO);
	glDeleteTextures(S_NumTextures, &S_Textures[0]);

	glGenFramebuffers(1, &S_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, S_FBO);

	//Optimize Bit Depths Later
	for (int i = 0; i < S_NumTextures; i++) {
		glGenTextures(1, &S_Textures[i]);
		glBindTexture(GL_TEXTURE_2D, S_Textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WindowWidth, WindowHeight, 0, GL_RGBA, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, S_Textures[i], 0);
		glGenerateMipmap(GL_TEXTURE_2D); 
	}

	GLuint attachments[8] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3,
		GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7 };

	glDrawBuffers(8, attachments);

	glGenRenderbuffers(1, &S_DepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, S_DepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, S_DepthBuffer);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "Frame buffer not complete!" << std::endl;
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void FrontBufferObject::BindTextures(Shader* Shader) {
	for (int i = 0; i < GetNumTextures(); i++) {
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, GetTexture(i));
		glUniform1i(glGetUniformLocation(Shader->GetProgram(), GetTextureName(i).c_str()), i);
	}
}

void FrontBufferObject::BindForWriting() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, S_FBO);
}
void FrontBufferObject::BindForReading() {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, S_FBO);
}
void FrontBufferObject::SetReadBuffer(int TextureIndex) {
	glReadBuffer(GL_COLOR_ATTACHMENT0 + TextureIndex);
}
