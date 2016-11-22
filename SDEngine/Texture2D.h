#pragma once
#include <string>
#include <GLEW/glew.h>

class Texture2D {

public:
	/*Wrap Behaviour: GL_REPEAT or GL_CLAMP. Filter Behaviour: GL_LINEAR or GL_NEAREST*/
	Texture2D(const std::string& FileName, const std::string& Type = "tex_albedo", unsigned int ExpectedComponent = 4, GLint WrapBehaviour = GL_REPEAT, GLfloat FilterBehaviour = GL_LINEAR);
	virtual ~Texture2D();

	void Bind(unsigned int Unit);

	std::string GetType() { return S_Type; }
	int GetUnit() { return S_Unit; }
	GLuint& GetTexture() { return S_Texture; }

private:
	GLuint S_Texture;
	std::string S_Type;
	int S_Unit;
	std::string S_FileName;
};

