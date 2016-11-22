#pragma once
#include <string>
#include <GLEW/glew.h>
#include "Transform.h"
#include "Camera.h"

enum class EShaderType {
	VERTEX,
	FRAGMENT
};
class Shader {

public:
	Shader() {}
	Shader(const std::string& ShaderName);
	virtual ~Shader();

	void Bind();
	void Update(const Transform& Transform, Camera& Camera);
	void RecompileShader();

	GLuint S_Program;

private:
	enum {
		MODEL_MATRIX_U,
		MVP_MATRIX_U,
		AMBIENT_COLOR_U,
		AMBIENT_INTENSITY_U,
		LIGHT_DIRECTION_U,
		LIGHT_COLOR_U,
		LIGHT_INTENSITY_U,
		CAMERA_VIEW_POSITION_U,
		NUM_UNIFORMS
	};
	enum {
		VERTEX_SHADER,
		FRAGMENT_SHADER,

		NUM_SHADERS
	};
	std::string ShaderName;
	std::string LoadShader(const std::string& fileName);
	GLuint CreateShader(const std::string& text, GLenum ShaderType);
	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);


	GLuint S_Shaders[NUM_SHADERS];
	GLuint S_Uniforms[NUM_UNIFORMS];
};

