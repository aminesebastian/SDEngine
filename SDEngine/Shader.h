#pragma once
#include <string>
#include <GLEW/glew.h>
#include <string>
#include "Transform.h"
#include "Texture2D.h"
#include "TypeDefenitions.h"

class Camera;

enum class EShaderType {
	VERTEX,
	FRAGMENT
};
using namespace std;
class Shader {

public:
	Shader() {}
	Shader(const std::string& ShaderName, bool bUseDefaultGeometry = true);
	virtual ~Shader();

	void Bind();
	void Update(const Transform& Transform, Camera* Camera);
	void RecompileShader();

	void SetShaderInteger(string Name, int Value);
	void SetShaderVector4(string Name, vec4 Vector);
	void SetShaderVector3(string Name, vec3 Vector);
	void SetShaderVector2(string Name, vec2 Vector);
	void SetShaderFloat(string Name, float Value);
	void SetShaderMatrix4(string Name, mat4 Matrix);
	void SetShaderTexture(string Name, Texture2D* Texture, GLuint Sample);

	GLuint& GetProgram() { return S_Program; }

private:
	enum {
		MODEL_MATRIX_U,
		MVP_MATRIX_U,
		AMBIENT_COLOR_U,
		AMBIENT_INTENSITY_U,
		LIGHT_DIRECTION_U,
		LIGHT_COLOR_U,
		LIGHT_INTENSITY_U,
		CAMERA_VIEW_VECTOR_U,
		NUM_UNIFORMS
	};
	enum {
		VERTEX_SHADER,
		FRAGMENT_SHADER,

		NUM_SHADERS
	};
	TString S_FragmentShaderPath;
	TString S_VertexShaderPath;
	TString S_ShaderName;

	std::string LoadShader(const std::string& fileName);
	GLuint CreateShader(const std::string& text, GLenum ShaderType);
	bool CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);

	GLuint S_Program;
	GLuint S_Shaders[NUM_SHADERS];
};

