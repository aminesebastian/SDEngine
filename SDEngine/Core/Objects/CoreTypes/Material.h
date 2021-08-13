#pragma once
#include "Core/Objects/CoreTypes/Shader.h"
#include "Core/Objects/CoreTypes/Texture2D.h"
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Assets/ISerializeableAsset.h"
#include <vector>

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

class Entity;

class Material : public ISerializeableAsset {

public:
	Material(string BaseShaderName);
	Material(Shader* Shader);
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
	void BindMaterial(const FTransform& RenderTransform, const Camera* RenderCamera);
	void BindMaterial(const FTransform& RenderTransform, const FTransform& LastFrameTransform, const Camera* RenderCamera);

	bool ImportAsset(const TString& FilePath) override { return true; }
	bool SerializeToBuffer(SerializationStream& Stream) const override;
	bool DeserializeFromBuffer(DeserializationStream& Stream) override;
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

