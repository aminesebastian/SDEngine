#include "Shader.h"
#include <iostream>
#include <fstream>
#include "Camera.h"

Shader::Shader(const std::string& ShaderName) {
	this->ShaderName = ShaderName;
	RecompileShader();
}
void Shader::RecompileShader() {
	S_Program = glCreateProgram();

	S_Shaders[0] = CreateShader(LoadShader(ShaderName + ".vert"), GL_VERTEX_SHADER);
	S_Shaders[1] = CreateShader(LoadShader(ShaderName + ".frag"), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < NUM_SHADERS; i++) {
		glAttachShader(S_Program, S_Shaders[i]);
	}

	glBindAttribLocation(S_Program, 0, "position");
	glBindAttribLocation(S_Program, 1, "texCoord");
	glBindAttribLocation(S_Program, 2, "normal");
	glBindAttribLocation(S_Program, 3, "vertexColor");

	glLinkProgram(S_Program);
	CheckShaderError(S_Program, GL_LINK_STATUS, true, "ERROR: Program Linking Failed: ");

	glValidateProgram(S_Program);
	CheckShaderError(S_Program, GL_VALIDATE_STATUS, true, "ERROR: Program Validation Failed: ");

	int lastSlash = 0;
	for (int i = 0; i < ShaderName.length(); i++) {
		if (ShaderName[i] == '/') {
			lastSlash = i;
		}
	}
	std::cout << "Compilation Success! -> " << ShaderName.substr(lastSlash+1, ShaderName.length() - lastSlash) << std::endl;
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

void Shader::Bind() {
	glUseProgram(S_Program);
}
void Shader::Update(const Transform& Transform, Camera* Camera) {

	mat4 tempMVP = Camera->GetProjectionMatrix()* Camera->GetViewMatrix() * Transform.GetModelMatrix();

	SetShaderMatrix4("MODEL_MATRIX", Transform.GetModelMatrix());
	SetShaderMatrix4("MVP", tempMVP);
	SetShaderFloat("NEAR_CLIP", Camera->GetNearClipPlane());
	SetShaderFloat("FAR_CLIP", Camera->GetFarClipPlane());
}


void Shader::SetShaderInteger(string Name, int Value) {
	glUniform1i(glGetUniformLocation(GetProgram(), Name.c_str()), Value);
}
void Shader::SetShaderVector4(string Name, vec4 Vector) {
	glUniform4fv(glGetUniformLocation(GetProgram(), Name.c_str()), 1, &Vector[0]);
}
void Shader::SetShaderVector3(string Name, vec3 Vector) {
	glUniform3fv(glGetUniformLocation(GetProgram(), Name.c_str()), 1, &Vector[0]);
}
void Shader::SetShaderVector2(string Name, vec2 Vector) {
	glUniform2fv(glGetUniformLocation(GetProgram(), Name.c_str()), 1, &Vector[0]);
}
void Shader::SetShaderFloat(string Name, float Value) {
	glUniform1f(glGetUniformLocation(GetProgram(), Name.c_str()), Value);
}
void Shader::SetShaderMatrix4(string Name, mat4 Matrix) {
	glUniformMatrix4fv(glGetUniformLocation(GetProgram(), Name.c_str()), 1, GL_FALSE, &Matrix[0][0]);
}
void Shader::SetShaderTexture(string Name, Texture2D* Texture, GLuint Sample) {
	glUniform1i(glGetUniformLocation(GetProgram(), Name.c_str()), Sample);
	Texture->Bind(Sample);
}