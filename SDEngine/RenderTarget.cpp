#include "RenderTarget.h"
#include <iostream>
#include "Shader.h"

RenderTarget::RenderTarget() {
	S_TextureCount = 0;
	S_FBO = 0;
}
RenderTarget::~RenderTarget() {
	if (S_FBO != 0) {
		glDeleteFramebuffers(1, &S_FBO);
	}
	if (S_Textures[0] != 0) {
		glDeleteTextures(S_TextureCount, &S_Textures[0]);
	}
}

void RenderTarget::AddTextureIndex(FRenderTargetTextureEntry* Texture) {
	if(S_TextureCount < 8) {
		S_TextureEntries.push_back(Texture);
		S_TextureCount++;
	}
}
bool RenderTarget::Init(unsigned int WindowWidth, unsigned int WindowHeight) {
	glDeleteFramebuffers(1, &S_FBO);
	glDeleteTextures(S_TextureCount, &S_Textures[0]);

	glGenFramebuffers(1, &S_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, S_FBO);

	//Optimize Bit Depths Later
	for (int i = 0; i < S_TextureCount; i++) {
		glGenTextures(1, &S_Textures[i]);
		glBindTexture(GL_TEXTURE_2D, S_Textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, S_TextureEntries[i]->S_InternalFormat, WindowWidth, WindowHeight, 0, S_TextureEntries[i]->S_Format, S_TextureEntries[i]->S_DataType, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, S_TextureEntries[i]->S_MinMagFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, S_TextureEntries[i]->S_MinMagFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, S_TextureEntries[i]->S_WrapType);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, S_TextureEntries[i]->S_WrapType);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, S_Textures[i], 0);
		glGenerateMipmap(GL_TEXTURE_2D); 
	}
	SArray<GLuint> attachments;
	for (int i = 0; i < S_TextureCount; i++) {
		attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
	}

	glDrawBuffers(S_TextureCount, &attachments[0]);

	glGenTextures(1, &S_DepthBuffer);
	glBindTexture(GL_TEXTURE_2D, S_DepthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, S_DepthBuffer, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "Frame buffer not complete!" << std::endl;
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	S_FBOSize = vec2(WindowWidth, WindowHeight);

	return true;
}

void RenderTarget::BindTextures(Shader* Shader, int Offset, bool bIncludeDepth) {
	for (int i = 0; i < GetTextureCount(); i++) {
		glActiveTexture(GL_TEXTURE0 + i + Offset);
		glBindTexture(GL_TEXTURE_2D, GetTexture(i));
		glUniform1i(glGetUniformLocation(Shader->GetProgram(), GetTextureName(i).c_str()), i + Offset);
		glBindSampler(i + Offset, GetTexture(i));
	}
}
void RenderTarget::BindTexture(Shader* Shader, int Index, int Offset, TString Name) {
	TString name = Name == "" ? GetTextureName(Index) : Name;
	glActiveTexture(GL_TEXTURE0 + Offset);
	glBindTexture(GL_TEXTURE_2D, GetTexture(Index));
	glUniform1i(glGetUniformLocation(Shader->GetProgram(), name.c_str()), Offset);
	glBindSampler(Offset, GetTexture(Index));
}

void RenderTarget::BindForWriting() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, S_FBO);
}
void RenderTarget::BindForReading() {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, S_FBO);
}
void RenderTarget::SetReadBuffer(int TextureIndex) {
	glReadBuffer(GL_COLOR_ATTACHMENT0 + TextureIndex);
}
