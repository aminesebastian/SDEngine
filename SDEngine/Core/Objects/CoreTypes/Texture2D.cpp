#include "Texture2D.h"
#include "Core/Utilities/Logger.h"
#include "Core/Utilities/Serialization/DeserializationStream.h"
#include "Core/Utilities/Serialization/DeserializationStream.h"
#include "Core/Utilities/stbi.h"
#include "Shader.h"
#include <cassert>

Texture2D::Texture2D(const TString& TextureName) : EngineObject(TextureName) {
	bSentToGPU = false;
	ExpectedComponents = 4;
	FileName = "";
	WrapBehaviour = GL_REPEAT;
	FilterBehaviour = GL_LINEAR;
	Texture = 0;
}
Texture2D::Texture2D(const TString& TextureName, const TString& FileName, int32 ExpectedComponents, GLint WrapBehaviour, GLfloat FilterBehaviour) : Texture2D(TextureName) {
	this->ExpectedComponents = ExpectedComponents;
	this->FileName        = FileName;
	this->WrapBehaviour   = WrapBehaviour;
	this->FilterBehaviour = FilterBehaviour;
	LoadTexture();
}
Texture2D::Texture2D(const TString& TextureName, int32 Width, int32 Height) : Texture2D(TextureName) {
	TextureDimensions = vec2(Width, Height);
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
}
Texture2D::~Texture2D() {
	if (bSentToGPU) {
		glDeleteTextures(1, &Texture);
	}
}

void Texture2D::Bind(TString Name, Shader* BindShader, unsigned int Offset) {
	if (TextureData.IsEmpty()) {
		SD_ENGINE_ERROR("Attempted to bind a texture where no data is present with name: {0} with file path: {1}.", Name, FileName);
		return;
	}
	if (!bSentToGPU) {
		SD_ENGINE_WARN("Attempted to bind a texture that has not been allocated on the GPU with name: {0} with file path: {1}. Sending to the GPU now.", Name, FileName);
		SendTextureDataToGPU();
	}

	assert(Offset <= 31);
	//CHANGED FOR PERF glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0 + Offset);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glUniform1i(glGetUniformLocation(BindShader->GetProgram(), Name.c_str()), Offset);
}
const GLuint& Texture2D::GetTexture() const {
	return Texture; 
}
const vec2& Texture2D::GetDimensions() const {
	return TextureDimensions;
}

void Texture2D::LoadTexture() {
	// If the texture was sent to the GPU, delete it.
	if (bSentToGPU) {
		glDeleteTextures(1, &Texture);
	}
	int32 width, height;

	// Load texture from file.
	unsigned char* image = stbi_load((FileName).c_str(), &width, &height, &NumComponents, ExpectedComponents);

	// If the load failed, log it and mark the TextureData as a nullptr.
	if (!image) {
		SD_ENGINE_ERROR("Unable to read texture from file: {0}", FileName);
		return;
	}

	// Place data into texture data array.
	TextureData.PreAllocate((width * height) * 4);

	for (int i = 0; i < (width * height) * 4; i++) {
		TextureData.Add(image[i]);
	}

	// Capture texture dimensions.
	TextureDimensions = vec2(width, height);

	// Free the image data buffer.
	stbi_image_free(image);
}
void Texture2D::SendTextureDataToGPU() {
	if (TextureData.IsEmpty()) {
		SD_ENGINE_WARN("Attempted to send a texture to the GPU where no data was present.");
		return;
	}
	if (Texture != 0) {
		glDeleteTextures(1, &Texture);
	}

	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapBehaviour);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapBehaviour);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilterBehaviour);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilterBehaviour);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (int32)TextureDimensions.x, (int32)TextureDimensions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, &TextureData[0]);
	glGenerateMipmap(GL_TEXTURE_2D);

	bSentToGPU = true;
	SD_ENGINE_INFO("Sent texture: {0} to GPU", GetName());
}

bool Texture2D:: SerializeToBuffer(SerializationStream& Stream) const {
	Stream.SerializeVec2(TextureDimensions);
	Stream.SerializeInteger32(NumComponents);
	Stream.SerializeUnsignedCharacterArray(TextureData);
	return true;
}
bool Texture2D::DeserializeFromBuffer(DeserializationStream& Stream) {
	Stream.DeserializeVec2(TextureDimensions);
	Stream.DeserializeInteger32(NumComponents);
	Stream.DeserializeUnsignedCharacterArray(TextureData);
	return true;
}