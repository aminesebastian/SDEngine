#include "Texture2D.h"
#include <cassert>
#include <iostream>
#include "ImageUtils.h"


Texture2D::Texture2D(const std::string& FileName, unsigned int ExpectedComponents, GLint WrapBehaviour, GLfloat FilterBehaviour) {
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

	//glGenerateMipmap(GL_TEXTURE_2D);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	stbi_image_free(imageData);
}
Texture2D::~Texture2D() {
	glDeleteTextures(1, &S_Texture);
}

void Texture2D::Bind(unsigned int Unit) {
	assert(Unit >= 0 && Unit <= 31);

	glActiveTexture(GL_TEXTURE0 + Unit);
	glBindTextureUnit(GL_TEXTURE_2D, S_Texture);	
}