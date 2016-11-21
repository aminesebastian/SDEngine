#pragma once
#include <string>
#include <GLEW/glew.h>

class Texture2D {

public:
	/*Wrap Behaviour: GL_REPEAT or GL_CLAMP. Filter Behaviour: GL_LINEAR or GL_NEAREST*/
	Texture2D(const std::string& FileName, unsigned int ExpectedComponent = 4, GLint WrapBehaviour = GL_REPEAT, GLfloat FilterBehaviour = GL_LINEAR, const std::string& Type = "tex_albedo", int ID = 0);
	virtual ~Texture2D();

	void Bind(unsigned int Unit);
	std::string GetType() { return Type; }
	int GetID() { return ID; }

private:
	GLuint S_Texture;
	std::string Type;
	int ID;
};

