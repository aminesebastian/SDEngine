#pragma once
#include <GLEW/glew.h>
#include "Utilities/TypeDefenitions.h"
#include "Utilities/Transform.h"

class Camera;
class Texture2D;

enum class EShaderType {
	VERTEX,
	FRAGMENT
};

class Shader {

public:
	Shader();
	Shader(const TString& ShaderName);
	Shader(const TString& FragmentShaderPath, const TString& VertexShaderPath);
	virtual ~Shader();

	void Bind();
	void Update(const class Transform& RenderTransform, Camera* Camera);
	void Update(const class Transform& RenderTransform,  const class Transform& LastFrameTrasnform, Camera* Camera);
	void RecompileShader();

	void SetShaderInteger(TString Name, int Value);
	void SetShaderVector4(TString Name, vec4 Vector);
	void SetShaderVector3(TString Name, vec3 Vector);
	void SetShaderVector2(TString Name, vec2 Vector);
	void SetShaderFloat(TString Name, float Value);
	void SetShaderMatrix3(TString Name, mat3 Matrix);
	void SetShaderMatrix4(TString Name, mat4 Matrix);
	void SetShaderTexture(TString Name, Texture2D* Texture, int32 Offset);

	TString GetFramentShaderFilePath();
	TString GetVertexShaderFilePath();

	TString StripDirectiveName(TString RawLine);

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

	TString LoadShader(const TString& fileName);
	GLuint CreateShader(const TString& text, GLenum ShaderType);
	bool CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const TString& errorMessage);

	GLuint S_Program;
	GLuint S_Shaders[NUM_SHADERS];
};
