#include "Texture2D.h"
#include "ImageUtils.h"


Texture2D::Texture2D(const std::string& FileName, unsigned int ExpectedComponents, GLint WrapBehaviour, GLfloat FilterBehaviour) {
	int width, height, numComponents;
	unsigned char* imageData = stbi_load(FileName.c_str(), &width, &height, &numComponents, ExpectedComponents);

	glGenTextures(1, &S_Texture);
	glBindTexture(GL_TEXTURE_2D, S_Texture);

	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapBehaviour);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapBehaviour);

	glTextureParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilterBehaviour);
	glTextureParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilterBehaviour);

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

	stbi_image_free(imageData);
}
Texture2D::~Texture2D() {

}

void Texture2D::Bind(unsigned int Unit) {
	glBindTextureUnit(Unit, S_Texture);
	
}