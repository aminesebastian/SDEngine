#pragma once
#include <string>
#include <GLEW/glew.h>

class Texture2D {

public:
	/*Wrap Behaviour: GL_REPEAT or GL_CLAMP. Filter Behaviour: GL_LINEAR or GL_NEAREST*/
	Texture2D(const std::string& FileName,  unsigned int ExpectedComponent = 4, GLint WrapBehaviour = GL_REPEAT, GLfloat FilterBehaviour = GL_LINEAR);
	Texture2D(int Width, int Height);
	virtual ~Texture2D();

	void Bind(unsigned int Unit);

	int GetUnit() { return S_Unit; }
	GLuint& GetTexture() { return S_Texture; }

private:
	GLuint S_Texture;
	int S_Unit;
	std::string S_FileName;
};

