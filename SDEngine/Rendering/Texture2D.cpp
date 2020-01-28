#include "Texture2D.h"
#include <cassert>
#include "Utilities/ImageUtils.h"
#include "Utilities/Logger.h"
#include "Shader.h"
#include "Core\Utilities\SerializationStream.h"
#include "Core\Utilities\DeserializationStream.h"

Texture2D::Texture2D(const TString& FileName, int32 ExpectedComponents, GLint WrapBehaviour, GLfloat FilterBehaviour) {
	bSentToGPU = false;
	this->ExpectedComponents = ExpectedComponents;
	this->FileName        = FileName;
	this->WrapBehaviour   = WrapBehaviour;
	this->FilterBehaviour = FilterBehaviour;
	LoadTexture();
}
Texture2D::Texture2D(int Width, int Height) {
	bSentToGPU = false;
	this->Width = Width;
	this->Height = Height;
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
}
Texture2D::~Texture2D() {
	if (bSentToGPU) {
		glDeleteTextures(1, &Texture);
	}
}

void Texture2D::Bind(TString Name, Shader* Shader, unsigned int Offset) {
	if (TextureData.IsEmpty()) {
		SD_ENGINE_ERROR("Attempted to bind a texture where no data is present with name: {0} with file path: {1}.", Name, FileName);
		return;
	}
	if (!bSentToGPU) {
		SD_ENGINE_ERROR("Attempted to bind a texture that has not been allocated on the GPU with name: {0} with file path: {1}.", Name, FileName);
		return;
	}

	assert(Offset <= 31);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0 + Offset);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glUniform1i(glGetUniformLocation(Shader->GetProgram(), Name.c_str()), Offset);
}

void Texture2D::LoadTexture() {
	// If the texture was sent to the GPU, delete it.
	if (bSentToGPU) {
		glDeleteTextures(1, &Texture);
	}

	// Load texture from file.
	unsigned char* data = stbi_load((FileName).c_str(), &Width, &Height, &NumComponents, ExpectedComponents);

	// If the load failed, log it and mark the TextureData as a nullptr.
	if (!data) {
		SD_ENGINE_ERROR("Unable to read texture from file: {0}", FileName);
		return;
	}

	// Place data into texture data array.
	TextureData.PreAllocate(Width * Height);
	TextureData.Add('a');
	memcpy(&TextureData[0], data, sizeof(char) * ((int32)Width * Height));
	delete data;
}
void Texture2D::SendTextureDataToGPU() {
	if (TextureData.IsEmpty()) {
		SD_ENGINE_WARN("Attempted to send a texture to the GPU where no data was present.");
		return;
	}
	if (&Texture != 0) {
		glDeleteTextures(1, &Texture);
		return;
	}

	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapBehaviour);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapBehaviour);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilterBehaviour);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilterBehaviour);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &TextureData[0]);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTextureUnit(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE0);

	bSentToGPU = true;
	SD_ENGINE_INFO("Send texture: {0} to GPU", FileName);
}

bool Texture2D::SerializeToBuffer(ByteBuffer& Buffer) const {
	SerializationStream ss(Buffer);
	ss.SerializeCharacterArray(TextureData);
	return true;
}
bool Texture2D::DeserializeFromBuffer(const ByteBuffer& Buffer) {
	DeserializationStream ds(Buffer);
	ds.DeserializeCharacterArray(TextureData);
	return true;
}