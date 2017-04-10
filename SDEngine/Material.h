#pragma once
#include "Shader.h"
#include "Texture2D.h"
#include <string>
#include <vector>
extern "C" {
#include "SLogger.h"
}

using namespace std;

struct FTextureParameter {
	string		Name;
	Texture2D*	Texture;
};
struct FVec4Parameter {
	string		Name;
	vec4		Vector;
};
struct FVec3Parameter {
	string		Name;
	vec3		Vector;
};
struct FVec2Parameter {
	string		Name;
	vec2		Vector;
};
struct FScalarParameter {
	string		Name;
	float		Scalar;
};
struct FBoolParameter {
	string		Name;
	bool		Value;
};
enum EShaderModel {
	DEFAULT,
	UNLIT,
	TRANSLUCENT,
	SSS
};

class Material {

public:
	Material(string BaseShaderName);
	~Material();

	Shader* GetShader() { return S_Shader; }
	
	bool SetVec4Parameter(string Name, vec4 Value);
	bool SetVec3Parameter(string Name, vec3 Value);
	bool SetVec2Parameter(string Name, vec2 Value);
	bool SetScalarParameter(string Name, float Value);
	bool SetBoolParameter(string Name, bool Value);
	bool SetTextureParameter(string Name, Texture2D* Texture);

	bool SetShaderModel(EShaderModel Model);
	EShaderModel GetShaderModel() { return S_ShaderModel; }
	void BindMaterial(Transform EntityTransform, Camera* Camera);

private:
	Shader* S_Shader;
	EShaderModel S_ShaderModel;


	vector<FTextureParameter> S_TextureParameters;
	vector<FVec4Parameter> S_Vec4Parameters;
	vector<FVec3Parameter> S_Vec3Parameters;
	vector<FVec2Parameter> S_Vec2Parameters;
	vector<FScalarParameter> S_ScalarParameters;
	vector<FBoolParameter> S_BoolParameters;

};

