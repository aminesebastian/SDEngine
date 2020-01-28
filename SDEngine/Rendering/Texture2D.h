#pragma once
#include <GLEW/glew.h>
#include "Core/DataTypes/TypeDefenitions.h"

class Shader;

class Texture2D {

public:
	/*Wrap Behaviour: GL_REPEAT or GL_CLAMP. Filter Behaviour: GL_LINEAR or GL_NEAREST*/
	Texture2D(const std::string& FileName,  unsigned int ExpectedComponent = 4, GLint WrapBehaviour = GL_REPEAT, GLfloat FilterBehaviour = GL_LINEAR);
	Texture2D(int Width, int Height);
	virtual ~Texture2D();

	void Bind(TString Name, Shader* Shader, uint32 Unit);

	GLuint& GetTexture() { return S_Texture; }

private:
	GLuint S_Texture;
	std::string S_FileName;
};

