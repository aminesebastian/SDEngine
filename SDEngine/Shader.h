#pragma once
#include <string>
#include <GLEW/glew.h>

enum class EShaderType {
	VERTEX,
	FRAGMENT
};
class Shader {

public:
	Shader(const std::string& ShaderName);
	virtual ~Shader();

	void Bind();

private:
	Shader(const Shader& other) {}
	void operator=(const Shader& other) {}

	std::string LoadShader(const std::string& fileName);
	GLuint CreateShader(const std::string& text, GLenum ShaderType);
	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);

	static const unsigned int NUM_SHADERS = 2; //Set to 3 for Geometry Shaders

	GLuint S_Program;
	GLuint S_Shaders[NUM_SHADERS];
};

