#include "Material.h"
#include <iostream>
#include "Engine/Engine.h"
#include "Entities/Light.h"
#include "Core\Utilities\SerializationStream.h"
#include "Core\Utilities\DeserializationStream.h"

Material::Material(const TString BaseShaderName) : Material(new Shader(BaseShaderName)) {}
Material::Material(Shader* Shader) {
	S_Shader = Shader;
	this->SetShaderModel(EShaderModel::DEFAULT);
}
Material::~Material() {

}

bool Material::SerializeToBuffer(ByteBuffer& Buffer) const {
	SerializationStream ss(Buffer);

	ss.SerializeString(S_Shader->GetFramentShaderFilePath());
	ss.SerializeString(S_Shader->GetVertexShaderFilePath());

	ss.SerializeInteger32(S_Vec4Parameters.Count());
	for (FVec4Parameter vector : S_Vec4Parameters) {
		ss.SerializeString(vector.Name);
		ss.SerializeVec4(vector.Vector);
	}
	ss.SerializeInteger32(S_Vec3Parameters.Count());
	for (FVec3Parameter vector : S_Vec3Parameters) {
		ss.SerializeString(vector.Name);
		ss.SerializeVec3(vector.Vector);
	}
	ss.SerializeInteger32(S_Vec2Parameters.Count());
	for (FVec2Parameter vector : S_Vec2Parameters) {
		ss.SerializeString(vector.Name);
		ss.SerializeVec2(vector.Vector);
	}
	ss.SerializeInteger32(S_ScalarParameters.Count());
	for (FScalarParameter vector : S_ScalarParameters) {
		ss.SerializeString(vector.Name);
		ss.SerializeFloat(vector.Scalar);
	}
	ss.SerializeInteger32(S_BoolParameters.Count());
	for (FBoolParameter vector : S_BoolParameters) {
		ss.SerializeString(vector.Name);
		ss.SerializeBool(vector.Value);
	}
	ss.SerializeInteger32(S_TextureParameters.Count());
	for (FTextureParameter vector : S_TextureParameters) {
		ss.SerializeString(vector.Name);
		ss.SerializeString(vector.Name);
	}

	return true;
}
bool Material::DeserializeFromBuffer(const ByteBuffer& Buffer) {
	DeserializationStream ds(Buffer);

	TString fragName;
	TString vertName;

	ds.DeserializeString(fragName);
	ds.DeserializeString(vertName);

	S_Shader = new Shader(fragName, vertName);

	int32 count = 0;
	ds.DeserializeInteger32(count);
	for (int i = 0; i < count; i++) {
		FVec4Parameter* param = new FVec4Parameter();
		TString name;
		vec4 value;
		ds.DeserializeString(name);
		ds.DeserializeVec4(value);
		param->Name = name;
		param->Vector = value;
		S_Vec4Parameters.Add(*param);
	}

	ds.DeserializeInteger32(count);
	for (int i = 0; i < count; i++) {
		FVec3Parameter* param = new FVec3Parameter();
		TString name;
		vec3 value;
		ds.DeserializeString(name);
		ds.DeserializeVec3(value);
		param->Name = name;
		param->Vector = value;
		S_Vec3Parameters.Add(*param);
	}

	ds.DeserializeInteger32(count);
	for (int i = 0; i < count; i++) {
		FVec2Parameter* param = new FVec2Parameter();
		TString name;
		vec2 value;
		ds.DeserializeString(name);
		ds.DeserializeVec2(value);
		param->Name = name;
		param->Vector = value;
		S_Vec2Parameters.Add(*param);
	}

	ds.DeserializeInteger32(count);
	for (int i = 0; i < count; i++) {
		FScalarParameter* param = new FScalarParameter();
		TString name;
		float value;
		ds.DeserializeString(name);
		ds.DeserializeFloat(value);
		param->Name = name;
		param->Scalar = value;
		S_ScalarParameters.Add(*param);
	}

	ds.DeserializeInteger32(count);
	for (int i = 0; i < count; i++) {
		FBoolParameter* param = new FBoolParameter();
		TString name;
		bool value;
		ds.DeserializeString(name);
		ds.DeserializeBool(value);
		param->Name = name;
		param->Value = value;
		S_BoolParameters.Add(*param);
	}

	ds.DeserializeInteger32(count);
	for (int i = 0; i < count; i++) {
		FTextureParameter* param = new FTextureParameter();
		TString name;
		TString tex;
		ds.DeserializeString(name);
		ds.DeserializeString(tex);
		param->Name = name;
		param->Texture = nullptr;
		S_TextureParameters.Add(*param);
	}


	return true;
}

bool Material::SetVec4Parameter(TString Name, vec4 Value) {
	for (int i = 0; i < this->S_Vec4Parameters.Count(); i++) {
		if (S_Vec4Parameters[i].Name == Name) {
			S_Vec4Parameters[i].Vector = Value;
		}
	}
	FVec4Parameter temp;
	temp.Name = Name;
	temp.Vector = Value;
	S_Vec4Parameters.Add(temp);
	return true;
}
bool Material::SetVec3Parameter(TString Name, vec3 Value) {
	for (int i = 0; i < this->S_Vec3Parameters.Count(); i++) {
		if (S_Vec3Parameters[i].Name == Name) {
			S_Vec3Parameters[i].Vector = Value;
		}
	}
	FVec3Parameter temp;
	temp.Name = Name;
	temp.Vector = Value;
	S_Vec3Parameters.Add(temp);
	return true;
}
bool Material::SetVec2Parameter(TString Name, vec2 Value) {
	for (int i = 0; i < this->S_Vec2Parameters.Count(); i++) {
		if (S_Vec2Parameters[i].Name == Name) {
			S_Vec2Parameters[i].Vector = Value;
		}
	}
	FVec2Parameter temp;
	temp.Name = Name;
	temp.Vector = Value;
	S_Vec2Parameters.Add(temp);
	return true;
}
bool Material::SetScalarParameter(TString Name, float Value) {
	for (int i = 0; i < this->S_ScalarParameters.Count(); i++) {
		if (S_ScalarParameters[i].Name == Name) {
			S_ScalarParameters[i].Scalar = Value;
		}
	}
	FScalarParameter temp;
	temp.Name = Name;
	temp.Scalar = Value;
	S_ScalarParameters.Add(temp);
	return true;
}
bool Material::SetBoolParameter(TString Name, bool Value) {
	for (int i = 0; i < this->S_BoolParameters.Count(); i++) {
		if (S_BoolParameters[i].Name == Name) {
			S_BoolParameters[i].Value = Value;
		}
	}
	FBoolParameter temp;
	temp.Name = Name;
	temp.Value = Value;
	S_BoolParameters.Add(temp);
	return true;
}
bool Material::SetTextureParameter(TString Name, Texture2D* Texture) {
	for (int i = 0; i < S_TextureParameters.Count(); i++) {
		if (S_TextureParameters[i].Name == Name) {
			S_TextureParameters[i].Texture = Texture;
		}
	}
	FTextureParameter newTexture;
	newTexture.Texture = Texture;
	newTexture.Name = Name;
	S_TextureParameters.Add(newTexture);
	return true;
}

bool Material::SetShaderModel(EShaderModel Model) {
	S_ShaderModel = Model;
	return true;
}
void Material::BindMaterial(const Transform& RenderTransform, Camera* RenderCamera) {
	BindMaterial(RenderTransform, RenderTransform, RenderCamera);
}
void Material::BindMaterial(const Transform& RenderTransform, const Transform& LastFrameTransform, Camera* RenderCamera) {
	S_Shader->Bind();
	S_Shader->Update(RenderTransform, LastFrameTransform, RenderCamera);
	glEnable(GL_TEXTURE_2D);

	S_Shader->SetShaderFloat("FRAME_TIME", (float)Engine::GetInstance()->GetFrameTime());
	S_Shader->SetShaderFloat("TIME", (float)Engine::GetInstance()->GetWorldTime());

	S_Shader->SetShaderInteger("MAT_ID", S_ShaderModel);
	for (int i = 0; i < this->S_TextureParameters.Count(); i++) {
		S_Shader->SetShaderTexture(S_TextureParameters[i].Name, S_TextureParameters[i].Texture, i);
	}
	for (int i = 0; i < this->S_Vec4Parameters.Count(); i++) {
		S_Shader->SetShaderVector4(S_Vec4Parameters[i].Name, S_Vec4Parameters[i].Vector);
	}
	for (int i = 0; i < this->S_Vec3Parameters.Count(); i++) {
		S_Shader->SetShaderVector3(S_Vec3Parameters[i].Name, S_Vec3Parameters[i].Vector);
	}
	for (int i = 0; i < this->S_Vec2Parameters.Count(); i++) {
		S_Shader->SetShaderVector2(S_Vec2Parameters[i].Name, S_Vec2Parameters[i].Vector);
	}
	for (int i = 0; i < this->S_ScalarParameters.Count(); i++) {
		S_Shader->SetShaderFloat(S_ScalarParameters[i].Name, S_ScalarParameters[i].Scalar);
	}
	for (int i = 0; i < this->S_BoolParameters.Count(); i++) {
		S_Shader->SetShaderInteger(S_BoolParameters[i].Name, S_BoolParameters[i].Value);
	}
	if (S_ShaderModel == EShaderModel::TRANSLUCENT) {
		for (int i = 0; i < Engine::GetInstance()->GetWorld()->GetWorldLights().Count(); i++) {
			Engine::GetInstance()->GetWorld()->GetWorldLights()[i]->SendShaderInformation(S_Shader, 1);
		}
	}
	glDisable(GL_TEXTURE_2D);
}