#include "Material.h"
#include <iostream>
#include "Core/Engine/Engine.h"
#include "Core/Objects/Entities/Light.h"
#include "Core/Utilities/Serialization/DeserializationStream.h"
#include "Core/Utilities/Serialization/DeserializationStream.h"

Material::Material(const std::string BaseShaderName) : Material(new Shader(BaseShaderName)) {}
Material::Material(Shader* Shader) {
	S_Shader = Shader;
	this->SetShaderModel(EShaderModel::DEFAULT);

	this->SetVec2Parameter("SCREEN_RES", vec2(WINDOW_WIDTH, WINDOW_HEIGHT));
	this->SetVec3Parameter("Normal", vec3(0.5f, 0.5f, 1.0f));
	this->SetScalarParameter("Roughness", 0.5f);
	this->SetScalarParameter("Metalness", 0.5f);
}
Material::~Material() {

}

bool Material::SetVec4Parameter(string Name, vec4 Value) {
	for (int i = 0; i < this->S_Vec4Parameters.size(); i++) {
		if (S_Vec4Parameters[i].Name == Name) {
			S_Vec4Parameters[i].Vector = Value;
		}
	}
	FVec4Parameter temp;
	temp.Name = Name;
	temp.Vector = Value;
	S_Vec4Parameters.push_back(temp);
	return true;
}
bool Material::SetVec3Parameter(string Name, vec3 Value) {
	for (int i = 0; i < this->S_Vec3Parameters.size(); i++) {
		if (S_Vec3Parameters[i].Name == Name) {
			S_Vec3Parameters[i].Vector = Value;
		}
	}
	FVec3Parameter temp;
	temp.Name = Name;
	temp.Vector = Value;
	S_Vec3Parameters.push_back(temp);
	return true;
}
bool Material::SetVec2Parameter(string Name, vec2 Value) {
	for (int i = 0; i < this->S_Vec2Parameters.size(); i++) {
		if (S_Vec2Parameters[i].Name == Name) {
			S_Vec2Parameters[i].Vector = Value;
		}
	}
	FVec2Parameter temp;
	temp.Name = Name;
	temp.Vector = Value;
	S_Vec2Parameters.push_back(temp);
	return true;
}
bool Material::SetScalarParameter(string Name, float Value) {
	for (int i = 0; i < this->S_ScalarParameters.size(); i++) {
		if (S_ScalarParameters[i].Name == Name) {
			S_ScalarParameters[i].Scalar = Value;
		}
	}
	FScalarParameter temp;
	temp.Name = Name;
	temp.Scalar = Value;
	S_ScalarParameters.push_back(temp);
	return true;
}
bool Material::SetBoolParameter(string Name, bool Value) {
	for (int i = 0; i < this->S_BoolParameters.size(); i++) {
		if (S_BoolParameters[i].Name == Name) {
			S_BoolParameters[i].Value = Value;
		}
	}
	FBoolParameter temp;
	temp.Name = Name;
	temp.Value = Value;
	S_BoolParameters.push_back(temp);
	return true;
}
bool Material::SetTextureParameter(string Name, Texture2D* Texture) {
	for (int i = 0; i < S_TextureParameters.size(); i++) {
		if (S_TextureParameters[i].Name == Name) {
			S_TextureParameters[i].Texture = Texture;
		}
	}
	FTextureParameter newTexture;
	newTexture.Texture = Texture;
	newTexture.Name = Name;
	S_TextureParameters.push_back(newTexture);
	return true;
}

bool Material::SetShaderModel(EShaderModel Model) {
	S_ShaderModel = Model;
	return true;
}
void Material::BindMaterial(const Transform& RenderTransform, const Camera* RenderCamera) {
	BindMaterial(RenderTransform, RenderTransform, RenderCamera);
}
void Material::BindMaterial(const Transform& RenderTransform, const Transform& LastFrameTransform, const Camera* RenderCamera) {
	S_Shader->Bind();
	S_Shader->Update(RenderTransform, LastFrameTransform, RenderCamera);
	glEnable(GL_TEXTURE_2D);

	S_Shader->SetShaderInteger("MAT_ID", S_ShaderModel);
	for (int i = 0; i < this->S_TextureParameters.size(); i++) {
		S_Shader->SetShaderTexture(S_TextureParameters[i].Name, S_TextureParameters[i].Texture, i);
	}
	for (int i = 0; i < this->S_Vec4Parameters.size(); i++) {
		S_Shader->SetShaderVector4(S_Vec4Parameters[i].Name, S_Vec4Parameters[i].Vector);
	}
	for (int i = 0; i < this->S_Vec3Parameters.size(); i++) {
		S_Shader->SetShaderVector3(S_Vec3Parameters[i].Name, S_Vec3Parameters[i].Vector);
	}
	for (int i = 0; i < this->S_Vec2Parameters.size(); i++) {
		S_Shader->SetShaderVector2(S_Vec2Parameters[i].Name, S_Vec2Parameters[i].Vector);
	}
	for (int i = 0; i < this->S_ScalarParameters.size(); i++) {
		S_Shader->SetShaderFloat(S_ScalarParameters[i].Name, S_ScalarParameters[i].Scalar);
	}
	for (int i = 0; i < this->S_BoolParameters.size(); i++) {
		S_Shader->SetShaderInteger(S_BoolParameters[i].Name, S_BoolParameters[i].Value);
	}
	glDisable(GL_TEXTURE_2D);
}

bool Material::SerializeToBuffer(SerializationStream& Stream) const {
	return true;
}
bool Material::DeserializeFromBuffer(DeserializationStream& Stream) {
	return true;
}