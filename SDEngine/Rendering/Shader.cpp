#include "Shader.h"
#include <fstream>
#include "Entities/Camera.h"
#include "Utilities/Logger.h"

Shader::Shader() {
	S_Program = 0;
}
Shader::Shader(const TString& ShaderName, bool bUseDefaultGeometry) {
	S_FragmentShaderPath = ShaderName + ".frag";
	if(bUseDefaultGeometry) {
		S_VertexShaderPath = "./Res/Shaders/DefaultGeometryPassShader.vert";
	}else{
		S_VertexShaderPath = ShaderName + ".vert";
	}
	int lastSlash = 0;
	for (int i = 0; i < S_FragmentShaderPath.length(); i++) {
		if (S_FragmentShaderPath[i] == '/') {
			lastSlash = i;
		}
	}
	S_ShaderName = S_FragmentShaderPath.substr(lastSlash + 1, S_FragmentShaderPath.length() - lastSlash - 5);
	RecompileShader();
}

void Shader::RecompileShader() {
	glDeleteProgram(S_Program);
	S_Program = glCreateProgram();

	glDeleteShader(S_Shaders[0]);
	glDeleteShader(S_Shaders[1]);
	S_Shaders[0] = CreateShader(LoadShader(S_VertexShaderPath), GL_VERTEX_SHADER);
	S_Shaders[1] = CreateShader(LoadShader(S_FragmentShaderPath), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < NUM_SHADERS; i++) {
		glAttachShader(S_Program, S_Shaders[i]);
	}

	glBindAttribLocation(S_Program, 0, "position");
	glBindAttribLocation(S_Program, 1, "texCoord");
	glBindAttribLocation(S_Program, 2, "normal");
	glBindAttribLocation(S_Program, 3, "vertexColor");

	bool success = true;
	glLinkProgram(S_Program);
	success = CheckShaderError(S_Program, GL_LINK_STATUS, true, "ERROR: Program Linking Failed For Shader:" + S_ShaderName + " ");

	if(!success) {
		return;
	}
	glValidateProgram(S_Program);
	success = CheckShaderError(S_Program, GL_VALIDATE_STATUS, true, "ERROR: Program Validation Failed: ");

	if (!success) {
		return;
	}
	SD_ENGINE_INFO("Shader Compilation Success: {0}", S_ShaderName)
}
Shader::~Shader() {
	for (unsigned int i = 0; i < NUM_SHADERS; i++) {
		glDetachShader(S_Program, S_Shaders[i]);
		glDeleteShader(S_Shaders[i]);
	}
	glDeleteProgram(S_Program);
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
			//TString trimmedLine = TStringUtils::Trim(line);
			////Add or remove directive from stack.
			//if (trimmedLine[0] == '#' && trimmedLine.length() > 1) {
			//	if (trimmedLine[1] == 'i') {
			//		precompilerStack.push(trimmedLine);
			//	}else if (trimmedLine[1] == 'e') {
			//		precompilerStack.pop();
			//	}
			//	continue;
			//}

			////Add line to shader lines.
			//if (precompilerStack.size() > 0) {
			//	if (S_Flags->find(precompilerStack.top()) != S_Flags->end()) {
			//		output.append(line + "\n");
			//	}
			//} else {
			//	output.append(line + "\n");
			//}
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
	glUseProgram(S_Program);
}
/************************************************************************/
/* Sets the Shader MODEL_MATRIX
/* Sets the shader MVP
/* Sets the NEAR_CLIP
/* Sets the FAR_CLIP
/* Sets the CAMERA_POS
/************************************************************************/
void Shader::Update(const class Transform& RenderTransform, Camera* Camera) {
	Update(RenderTransform, RenderTransform, Camera);
}
void Shader::Update(const class Transform& RenderTransform,  const class Transform& LastFrameTrasnform, Camera* Camera) {
	mat4 lastFrameMVP = Camera->GetProjectionMatrix() * Camera->GetLastFrameViewMatrix() * LastFrameTrasnform.GetModelMatrix();
	mat4 tempMVP = Camera->GetProjectionMatrix() * Camera->GetViewMatrix() * RenderTransform.GetModelMatrix();

	SetShaderMatrix3("NORMAL_MODEL_MATRIX", glm::transpose(glm::inverse(RenderTransform.GetModelMatrix())));
	SetShaderMatrix4("MODEL_MATRIX", RenderTransform.GetModelMatrix());
	SetShaderMatrix4("VIEW_MATRIX", Camera->GetViewMatrix());
	SetShaderMatrix4("PROJECTION_MATRIX", Camera->GetProjectionMatrix());
	SetShaderMatrix4("MVP", tempMVP);
	SetShaderMatrix4("LAST_MVP", lastFrameMVP);
	SetShaderFloat("NEAR_CLIP", Camera->GetNearClipPlane());
	SetShaderFloat("FAR_CLIP", Camera->GetFarClipPlane());

	SetShaderVector3("CAMERA_POS", Camera->GetTransform().GetLocation());
}
void Shader::SetShaderInteger(TString Name, int Value) {
	glUniform1i(glGetUniformLocation(GetProgram(), Name.c_str()), Value);
}
void Shader::SetShaderVector4(TString Name, vec4 Vector) {
	glUniform4fv(glGetUniformLocation(GetProgram(), Name.c_str()), 1, &Vector[0]);
}
void Shader::SetShaderVector3(TString Name, vec3 Vector) {
	glUniform3fv(glGetUniformLocation(GetProgram(), Name.c_str()), 1, &Vector[0]);
}
void Shader::SetShaderVector2(TString Name, vec2 Vector) {
	glUniform2fv(glGetUniformLocation(GetProgram(), Name.c_str()), 1, &Vector[0]);
}
void Shader::SetShaderFloat(TString Name, float Value) {
	glUniform1f(glGetUniformLocation(GetProgram(), Name.c_str()), Value);
}
void Shader::SetShaderMatrix3(TString Name, mat3 Matrix) {
	glUniformMatrix3fv(glGetUniformLocation(GetProgram(), Name.c_str()), 1, GL_FALSE, &Matrix[0][0]);
}
void Shader::SetShaderMatrix4(TString Name, mat4 Matrix) {
	glUniformMatrix4fv(glGetUniformLocation(GetProgram(), Name.c_str()), 1, GL_FALSE, &Matrix[0][0]);
}
void Shader::SetShaderTexture(TString Name, Texture2D* Texture, int32 Offset) {
	//glUniform1i(glGetUniformLocation(GetProgram(), Name.c_str()), Sample);
	Texture->Bind(Name.c_str(), this, Offset);
}