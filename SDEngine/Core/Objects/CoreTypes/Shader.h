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
	void Update(const Transform& RenderTransform, const Camera* RenderCamera);
	void Update(const Transform& RenderTransform,  const Transform& LastFrameTransform, const Camera* RenderCamera);
	void RecompileShader();

	void SetShaderInteger(const TString& Name, const int& Value);
	void SetShaderVector4(const TString& Name, const vec4& Vector);
	void SetShaderVector3(const TString& Name, const vec3& Vector);
	void SetShaderVector2(const TString& Name, const vec2& Vector);
	void SetShaderFloat(const TString& Name, const float& Value);
	void SetShaderMatrix3(const TString& Name, const mat3& Matrix);
	void SetShaderMatrix4(const TString& Name, const mat4& Matrix);
	void SetShaderTexture(const TString& Name, Texture2D* Texture, const int32& Offset);

	void SetShaderInteger(const GLuint& Location, const int& Value);
	void SetShaderVector4(const GLuint& Location, const vec4& Vector);
	void SetShaderVector3(const GLuint& Location, const vec3& Vector);
	void SetShaderVector2(const GLuint& Location, const vec2& Vector);
	void SetShaderFloat(const GLuint& Location, const float& Value);
	void SetShaderMatrix3(const GLuint& Location, const mat3& Matrix);
	void SetShaderMatrix4(const GLuint& Location, const mat4& Matrix);
	void SetShaderTexture(const GLuint& Location, Texture2D* Texture, const int32& Offset);

	GLuint& GetProgram() { return Program; }
private:
	enum {
		VERTEX_SHADER,
		FRAGMENT_SHADER,

		NUM_SHADERS
	};
	TString FragmentShaderFilePath;
	TString VertexShaderFilePath;
	TString Name;
	bool bCompiled;
	GLuint Program;
	GLuint Shaders[NUM_SHADERS];

	TString LoadShader(const TString& fileName);
	GLuint CreateShader(const TString& text, GLenum ShaderType);
	bool CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const TString& errorMessage);

};

