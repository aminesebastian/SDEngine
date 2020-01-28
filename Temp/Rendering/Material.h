#pragma once
#include "Rendering/Shader.h"
#include "Rendering/Texture2D.h"
#include "Core/Assets/ISerializeableAsset.h"
#include "Core/DataStructures/Array.h"
#include "Utilities/TypeDefenitions.h"

using namespace std;

struct FTextureParameter {
	TString		Name;
	Texture2D*	Texture;
};
struct FVec4Parameter {
	TString		Name;
	vec4		Vector;
};
struct FVec3Parameter {
	TString		Name;
	vec3		Vector;
};
struct FVec2Parameter {
	TString		Name;
	vec2		Vector;
};
struct FScalarParameter {
	TString		Name;
	float		Scalar;
};
struct FBoolParameter {
	TString		Name;
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
	Material(TString BaseShaderName);
	Material(Shader* Shader);
	~Material();

	Shader* GetShader() { return S_Shader; }
	
	bool SetVec4Parameter(TString Name, vec4 Value);
	bool SetVec3Parameter(TString Name, vec3 Value);
	bool SetVec2Parameter(TString Name, vec2 Value);
	bool SetScalarParameter(TString Name, float Value);
	bool SetBoolParameter(TString Name, bool Value);
	bool SetTextureParameter(TString Name, Texture2D* Texture);

	bool SetShaderModel(EShaderModel Model);
	EShaderModel GetShaderModel() { return S_ShaderModel; }
	void BindMaterial(const Transform& RenderTransform, Camera* RenderCamera);
	void BindMaterial(const Transform& RenderTransform, const Transform& LastFrameTransform, Camera* RenderCamera);

	bool SerializeToBuffer(ByteBuffer& Buffer) const override;
	bool DeserializeFromBuffer(const ByteBuffer& Buffer) override;
private:
	Shader* S_Shader;
	EShaderModel S_ShaderModel;

	SArray<FVec4Parameter> S_Vec4Parameters;
	SArray<FVec3Parameter> S_Vec3Parameters;
	SArray<FVec2Parameter> S_Vec2Parameters;
	SArray<FScalarParameter> S_ScalarParameters;
	SArray<FBoolParameter> S_BoolParameters;
	SArray<FTextureParameter> S_TextureParameters;
};

