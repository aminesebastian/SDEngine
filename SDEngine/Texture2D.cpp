#include "Texture2D.h"
#include <cassert>
#include <iostream>
#include "ImageUtils.h"


Texture2D::Texture2D(const std::string& FileName, const std::string& Type, unsigned int ExpectedComponents, GLint WrapBehaviour, GLfloat FilterBehaviour) {
	this->S_Type = Type;
	this->S_FileName = FileName;

	int width, height, numComponents;
	unsigned char* imageData = stbi_load((FileName).c_str(), &width, &height, &numComponents, ExpectedComponents);

	if (imageData == NULL)
		std::cerr << "Unable to load texture: " << FileName << std::endl;

	glGenTextures(1, &S_Texture);
	glBindTexture(GL_TEXTURE_2D, S_Texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapBehaviour);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapBehaviour);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilterBehaviour);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilterBehaviour);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	//glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(imageData);
	glBindTextureUnit(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE0);
}
Texture2D::Texture2D(int Width, int Height) {
	glGenTextures(1, &S_Texture);
	glBindTexture(GL_TEXTURE_2D, S_Texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
}
Texture2D::~Texture2D() {
	if (&S_Texture != 0) {
		glDeleteTextures(1, &S_Texture);
	}
}

void Texture2D::Bind(unsigned int Unit) {
	assert(Unit <= 31);
	this->S_Unit = Unit;
	glActiveTexture(GL_TEXTURE0 + S_Unit);
	glBindTexture(GL_TEXTURE_2D, S_Texture);
}