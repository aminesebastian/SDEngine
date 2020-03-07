#include "Shader.h"
#include "Core/Objects/CoreTypes/Texture2D.h"
#include "Core/Objects/Entities/Camera.h"
#include "Core/Utilities/Logger.h"
#include <fstream>

Shader::Shader() {
	bCompiled    = false;
	Program      = 0;
	Shaders[0]   = 0;
	Shaders[1]   = 0;
}
Shader::Shader(const TString& ShaderName, bool bUseDefaultGeometry) : Shader() {
	FragmentShaderFilePath = ShaderName + ".frag";
	if(bUseDefaultGeometry) {
		VertexShaderFilePath = "./Res/Shaders/DefaultGeometryPassShader.vert";
	}else{
		VertexShaderFilePath = ShaderName + ".vert";
	}
	int lastSlash = 0;
	for (int i = 0; i < FragmentShaderFilePath.length(); i++) {
		if (FragmentShaderFilePath[i] == '/') {
			lastSlash = i;
		}
	}
	Name = FragmentShaderFilePath.substr(lastSlash + 1, FragmentShaderFilePath.length() - lastSlash - 5);
	RecompileShader();
}

void Shader::RecompileShader() {
	// Check if this shader was previously compiled.
	if (bCompiled) {
		glDeleteProgram(Program);
		glDeleteProgram(Shaders[0]);
		glDeleteProgram(Shaders[1]);
	}

	Program = glCreateProgram();
	Shaders[0] = CreateShader(LoadShader(VertexShaderFilePath), GL_VERTEX_SHADER);
	Shaders[1] = CreateShader(LoadShader(FragmentShaderFilePath), GL_FRAGMENT_SHADER);

	glAttachShader(Program, Shaders[0]);
	glAttachShader(Program, Shaders[1]);

	glBindAttribLocation(Program, 0, "position");
	glBindAttribLocation(Program, 1, "texCoord");
	glBindAttribLocation(Program, 2, "normal");
	glBindAttribLocation(Program, 3, "vertexColor");

	bool success = true;
	glLinkProgram(Program);
	success = CheckShaderError(Program, GL_LINK_STATUS, true, "ERROR: Program Linking Failed For Shader:" + Name + " ");

	if(!success) {
		return;
	}
	glValidateProgram(Program);
	success = CheckShaderError(Program, GL_VALIDATE_STATUS, true, "ERROR: Program Validation Failed: ");

	if (!success) {
		return;
	}
	bCompiled = true;
	SD_ENGINE_INFO("Shader Compilation Success: {0}", Name)
}
Shader::~Shader() {
	for (unsigned int i = 0; i < NUM_SHADERS; i++) {
		glDetachShader(Program, Shaders[i]);
		glDeleteShader(Shaders[i]);
	}
	glDeleteProgram(Program);
}

TString Shader::LoadShader(const TString& fileName) {
	SStack<TString> precompilerStack;

	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open()){
		while (file.good()){
			getline(file, line);
			output.append(line + "\n");
		}
		file.close();
	}else{
		SD_ENGINE_ERROR("Unable to load shader: -> {0}", fileName)
		file.close();
	}

	return output;
}
GLuint Shader::CreateShader(const TString& text, GLenum ShaderType) {
	GLuint shader = glCreateShader(ShaderType);

	if (shader == 0) {
		SD_ENGINE_ERROR("Error compiling shader type: -> {0}", ShaderType)
	}

	const GLchar* shaderSourceStrings[1];
	shaderSourceStrings[0] = text.c_str();

	GLint shaderSourceStringLengths[1];
	shaderSourceStringLengths[0] = (GLint)text.length();

	glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "ERROR: Shader Compilation Failed: ");

	return shader;
}
bool Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const TString& errorMessage) {
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
		SD_ENGINE_ERROR("{0}: {1}", errorMessage, error)
	}
	return success;
}


void Shader::Bind() {
	glUseProgram(Program);
}
void Shader::Update(const FTransform& RenderTransform, const Camera* RenderCamera) {
	Update(RenderTransform, RenderTransform, RenderCamera);
}
void Shader::Update(const FTransform& RenderTransform,  const FTransform& LastFrameTransform, const Camera* RenderCamera) {
	mat4 lastFrameMVP = RenderCamera->GetProjectionMatrix() * RenderCamera->GetLastFrameViewMatrix() * LastFrameTransform.GetModelMatrix();
	mat4 tempMVP = RenderCamera->GetProjectionMatrix() * RenderCamera->GetViewMatrix() * RenderTransform.GetModelMatrix();

	SetShaderMatrix3("NORMAL_MODEL_MATRIX", glm::transpose(glm::inverse(RenderTransform.GetModelMatrix())));
	SetShaderMatrix4("MODEL_MATRIX", RenderTransform.GetModelMatrix());
	SetShaderMatrix4("VIEW_MATRIX", RenderCamera->GetViewMatrix());
	SetShaderMatrix4("PROJECTION_MATRIX", RenderCamera->GetProjectionMatrix());
	SetShaderMatrix4("MVP", tempMVP);
	SetShaderMatrix4("LAST_MVP", lastFrameMVP);
	SetShaderFloat("NEAR_CLIP", RenderCamera->GetNearClipPlane());
	SetShaderFloat("FAR_CLIP", RenderCamera->GetFarClipPlane());

	SetShaderVector3("CAMERA_POS", RenderCamera->GetTransform().GetLocation());
}
void Shader::SetShaderInteger(const TString& Name, const int& Value) {
	glUniform1i(glGetUniformLocation(GetProgram(), Name.c_str()), Value);
}
void Shader::SetShaderVector4(const TString& Name, const vec4& Vector) {
	glUniform4fv(glGetUniformLocation(GetProgram(), Name.c_str()), 1, &Vector[0]);
}
void Shader::SetShaderVector3(const TString& Name, const vec3& Vector) {
	glUniform3fv(glGetUniformLocation(GetProgram(), Name.c_str()), 1, &Vector[0]);
}
void Shader::SetShaderVector2(const TString& Name, const vec2& Vector) {
	glUniform2fv(glGetUniformLocation(GetProgram(), Name.c_str()), 1, &Vector[0]);
}
void Shader::SetShaderFloat(const TString& Name, const float& Value) {
	glUniform1f(glGetUniformLocation(GetProgram(), Name.c_str()), Value);
}
void Shader::SetShaderMatrix3(const TString& Name, const mat3& Matrix) {
	glUniformMatrix3fv(glGetUniformLocation(GetProgram(), Name.c_str()), 1, GL_FALSE, &Matrix[0][0]);
}
void Shader::SetShaderMatrix4(const TString& Name, const mat4& Matrix) {
	glUniformMatrix4fv(glGetUniformLocation(GetProgram(), Name.c_str()), 1, GL_FALSE, &Matrix[0][0]);
}
void Shader::SetShaderTexture(const TString& Name, Texture2D* Texture, const int32& Offset) {
	glActiveTexture(GL_TEXTURE0 + Offset);
	glBindTexture(GL_TEXTURE_2D, Texture->GetTexture());
	glUniform1i(glGetUniformLocation(Program, Name.c_str()), Offset);
}

void Shader::SetShaderInteger(const GLuint& Location, const int& Value) {
	glUniform1i(Location, Value);
}
void Shader::SetShaderVector4(const GLuint& Location, const vec4& Vector) {
	glUniform4fv(Location, 1, &Vector[0]);
}
void Shader::SetShaderVector3(const GLuint& Location, const vec3& Vector) {
	glUniform3fv(Location, 1, &Vector[0]);
}
void Shader::SetShaderVector2(const GLuint& Location, const vec2& Vector) {
	glUniform2fv(Location, 1, &Vector[0]);
}
void Shader::SetShaderFloat(const GLuint& Location, const float& Value) {
	glUniform1f(Location, Value);
}
void Shader::SetShaderMatrix3(const GLuint& Location, const mat3& Matrix) {
	glUniformMatrix3fv(Location, 1, GL_FALSE, &Matrix[0][0]);
}
void Shader::SetShaderMatrix4(const GLuint& Location, const mat4& Matrix) {
	glUniformMatrix4fv(Location, 1, GL_FALSE, &Matrix[0][0]);
}
void Shader::SetShaderTexture(const GLuint& Location, Texture2D* Texture, const int32& Offset) {
	glActiveTexture(GL_TEXTURE0 + Offset);
	glBindTexture(GL_TEXTURE_2D, Texture->GetTexture());
	glUniform1i(Location, Offset);
}