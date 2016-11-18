#pragma once
#include <string>
#include <GLEW/glew.h>

class Texture2D {

public:
	/*Wrap Behaviour: GL_REPEAT or GL_CLAMP. Filter Behaviour: GL_LINEAR or GL_NEAREST*/
	Texture2D(const std::string& FileName, unsigned int ExpectedComponent, GLint WrapBehaviour = GL_REPEAT, GLint FilterBehaviour = GL_LINEAR);
	virtual ~Texture2D();

	void Bind(unsigned int Unit);

private:
	Texture2D(const Texture2D& other) {}
	void operator=(const Texture2D& other) {}

	GLuint S_Texture;

};

