#include "Shader.h"
#include <iostream>
#include <fstream>


Shader::Shader(const std::string& ShaderName) {
	S_Program = glCreateProgram();

	S_Shaders[0] = CreateShader(LoadShader(ShaderName + ".vs"), GL_VERTEX_SHADER);
	S_Shaders[1] = CreateShader(LoadShader(ShaderName + ".fs"), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < NUM_SHADERS; i++) {
		glAttachShader(S_Program, S_Shaders[i]);
	}

	glBindAttribLocation(S_Program, 0, "position");
	glBindFragDataLocation(S_Program, 0, "outColor");

	glLinkProgram(S_Program);
	CheckShaderError(S_Program, GL_LINK_STATUS, true, "ERROR: Program Linking Failed: ");

	glValidateProgram(S_Program);
	CheckShaderError(S_Program, GL_VALIDATE_STATUS, true, "ERROR: Program Validation Failed: ");
}
Shader::~Shader() {
	for (unsigned int i = 0; i < NUM_SHADERS; i++) {
		glDetachShader(S_Program, S_Shaders[i]);
		glDeleteShader(S_Shaders[i]);
	}
	glDeleteProgram(S_Program);
}

std::string Shader::LoadShader(const std::string& fileName) {
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open()){
		while (file.good()){
			getline(file, line);
			output.append(line + "\n");
		}
	}else{
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
}
GLuint Shader::CreateShader(const std::string& text, GLenum ShaderType) {
	GLuint shader = glCreateShader(ShaderType);

	if (shader == 0) {
		std::cerr << "Error compiling shader type " << ShaderType << std::endl;
	}

	const GLchar* shaderSourceStrings[1];
	shaderSourceStrings[0] = text.c_str();

	GLint shaderSourceStringLengths[1];
	shaderSourceStringLengths[0] = text.length();

	glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "ERROR: Shader Compilation Failed: ");

	return shader;
}
void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage) {
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram) {
		glGetProgramiv(shader, flag, &success);
	}else {
		glGetShaderiv(shader, flag, &success);
	}

	if (success == GL_FALSE) {
		if (isProgram) {
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		}else{
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);
		}
		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}
GLuint Shader::GetShaderProgram() {
	return S_Program;
}

void Shader::Bind() {
	glUseProgram(S_Program);
}