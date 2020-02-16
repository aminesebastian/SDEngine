#pragma once
#include <GLEW/glew.h>
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Utilities/Math/Transform.h"

class Camera;
class Texture2D;

enum class EShaderType {
	VERTEX,
	FRAGMENT
};

class Shader {

public:
	Shader();
	Shader(const TString& ShaderName, bool bUseDefaultGeometry = true);
	virtual ~Shader();

	void Bind();
	void Update(const class Transform& RenderTransform, const Camera* RenderCamera);
	void Update(const class Transform& RenderTransform,  const class Transform& LastFrameTrasnform, const Camera* RenderCamera);
	void RecompileShader();

	void SetShaderInteger(const TString& Name, const int& Value);
	void SetShaderVector4(const TString& Name, const vec4& Vector);
	void SetShaderVector3(const TString& Name, const vec3& Vector);
	void SetShaderVector2(const TString& Name, const vec2& Vector);
	void SetShaderFloat(const TString& Name, const float& Value);
	void SetShaderMatrix3(const TString& Name, const mat3& Matrix);
	void SetShaderMatrix4(const TString& Name, const mat4& Matrix);
	void SetShaderTexture(const TString& Name, Texture2D* Texture, const int32& Offset);

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

	const GLuint& GetUniformLocation(const TString& UniformName);

	bool bCompiled;
	GLuint S_Program;
	GLuint S_Shaders[NUM_SHADERS];


	SHashMap<TString, GLuint> UniformMap;
};

