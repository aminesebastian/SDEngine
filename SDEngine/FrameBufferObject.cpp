#include "FrameBufferObject.h"
#include <iostream>
#include "Shader.h"

FrameBufferObject::FrameBufferObject() {
	S_NumTextures = 0;
	S_FBO = 0;
}
FrameBufferObject::~FrameBufferObject() {
	if (S_FBO != 0) {
		glDeleteFramebuffers(1, &S_FBO);
	}
	if (S_Textures[0] != 0) {
		glDeleteTextures(S_NumTextures, &S_Textures[0]);
	}
}

void FrameBufferObject::AddTextureIndex(FFBOTextureEntry* Texture) {
	if(S_NumTextures < 8) {
		S_TextureEntries.push_back(Texture);
		S_NumTextures++;
	}
}
bool FrameBufferObject::Init(unsigned int WindowWidth, unsigned int WindowHeight) {
	glDeleteFramebuffers(1, &S_FBO);
	glDeleteTextures(S_NumTextures, &S_Textures[0]);

	glGenFramebuffers(1, &S_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, S_FBO);

	//Optimize Bit Depths Later
	for (int i = 0; i < S_NumTextures; i++) {
		glGenTextures(1, &S_Textures[i]);
		glBindTexture(GL_TEXTURE_2D, S_Textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, S_TextureEntries[i]->S_InternalFormat, WindowWidth, WindowHeight, 0, S_TextureEntries[i]->S_Format, S_TextureEntries[i]->S_DataType, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, S_TextureEntries[i]->S_MinMagFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, S_TextureEntries[i]->S_MinMagFilter);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, S_TextureEntries[i]->S_WrapType);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, S_TextureEntries[i]->S_WrapType);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, S_Textures[i], 0);
		glGenerateMipmap(GL_TEXTURE_2D); 
	}

	GLuint attachments[8] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3,
		GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7 };

	glDrawBuffers(8, attachments);

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

	return true;
}

void FrameBufferObject::BindTextures(Shader* Shader, bool bIncludeDepth) {
	for (int i = 0; i < GetNumTextures(); i++) {
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, GetTexture(i));
		glUniform1i(glGetUniformLocation(Shader->GetProgram(), GetTextureName(i).c_str()), i);
	}
	if(bIncludeDepth) {
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0 + GetNumTextures());
		glBindTexture(GL_TEXTURE_2D, S_DepthBuffer);
		glUniform1i(glGetUniformLocation(Shader->GetProgram(), "depthBuffer"), GetNumTextures());
	}
}

void FrameBufferObject::BindForWriting() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, S_FBO);
}
void FrameBufferObject::BindForReading() {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, S_FBO);
}
void FrameBufferObject::SetReadBuffer(int TextureIndex) {
	glReadBuffer(GL_COLOR_ATTACHMENT0 + TextureIndex);
}
