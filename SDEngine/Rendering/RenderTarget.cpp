#include "RenderTarget.h"
#include <iostream>
#include "Rendering/Shader.h"

RenderTarget::RenderTarget(vec2 RenderTargetResolution) {
	TextureCount         = 0;
	FrameBuffer          = 0;
	DepthStencilBuffer   = 0;
	bGenerateDepthBuffer = false;
	Resolution           = RenderTargetResolution;
}
RenderTarget::~RenderTarget() {
	if (FrameBuffer != 0) {
		glDeleteFramebuffers(1, &FrameBuffer);
	}
	if (TextureAttachments[0] != 0) {
		glDeleteTextures(TextureCount, &TextureAttachments[0]);
	}
}

void RenderTarget::AddTextureIndex(FRenderTargetTextureEntry* Texture) {
	if (TextureCount < 8) {
		TextureEntries.Add(Texture);
		TextureCount++;
	}
}
void RenderTarget::AddDepthStencilBuffer() {
	bGenerateDepthBuffer = true;
}
bool RenderTarget::FinalizeRenderTarget() {
	glDeleteFramebuffers(1, &FrameBuffer);
	glDeleteTextures(TextureCount, &TextureAttachments[0]);

	glGenFramebuffers(1, &FrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer);

	//Optimize Bit Depths Later
	for (int i = 0; i < TextureCount; i++) {
		glGenTextures(1, &TextureAttachments[i]);
		glBindTexture(GL_TEXTURE_2D, TextureAttachments[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, TextureEntries[i]->S_InternalFormat, (GLsizei)Resolution.x, (GLsizei)Resolution.y, 0, TextureEntries[i]->S_Format, TextureEntries[i]->S_DataType, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TextureEntries[i]->S_MinMagFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TextureEntries[i]->S_MinMagFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, TextureEntries[i]->S_WrapType);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TextureEntries[i]->S_WrapType);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, TextureAttachments[i], 0);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	SArray<GLuint> attachments;
	for (int i = 0; i < TextureCount; i++) {
		attachments.Add(GL_COLOR_ATTACHMENT0 + i);
	}

	glDrawBuffers(TextureCount, &attachments[0]);


	if (bGenerateDepthBuffer) {
		glGenTextures(1, &DepthStencilBuffer);
		glBindTexture(GL_TEXTURE_2D, DepthStencilBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, (GLsizei)Resolution.x, (GLsizei)Resolution.y, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, DepthStencilBuffer, 0);
	}



	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "Frame buffer not complete!" << std::endl;
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Resolution = vec2(Resolution.x, Resolution.y);

	return true;
}

void RenderTarget::BindTextures(Shader* Shader, int32 Offset, bool bIncludeDepth) {
	for (int i = 0; i < GetAttachedTextureCount(); i++) {
		BindTexture(Shader, i, Offset + i);
	}
}
void RenderTarget::BindTexture(Shader* Shader, int32 Index, int32 Offset, TString Name) {
	TString name = Name == "" ? GetTextureName(Index) : Name;
	glActiveTexture(GL_TEXTURE0 + Offset);
	glBindTexture(GL_TEXTURE_2D, GetTexture(Index));
	glUniform1i(glGetUniformLocation(Shader->GetProgram(), name.c_str()), Offset);
}
void RenderTarget::BindDepthStencilTexture(Shader* Shader, int32 Offset, TString Name) {
	glActiveTexture(GL_TEXTURE0 + Offset);
	glBindTexture(GL_TEXTURE_2D, DepthStencilBuffer);
	glUniform1i(glGetUniformLocation(Shader->GetProgram(), Name.c_str()), Offset);
}

void RenderTarget::BindForWriting() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FrameBuffer);
}
void RenderTarget::BindForReading() {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, FrameBuffer);
}

GLuint& RenderTarget::GetTexture(int32 i) {
	assert(i < TextureCount);  return TextureAttachments[i];
}
TString RenderTarget::GetTextureName(int32 i) {
	return TextureEntries[i]->S_TextureName;
}
int32 RenderTarget::GetAttachedTextureCount() {
	return TextureCount;
}
vec2 RenderTarget::GetRenderTargetResolution() {
	return Resolution;
}