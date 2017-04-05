#include "Material.h"



Material::Material(const std::string BaseShaderName) {
	S_Shader = new Shader(BaseShaderName);
}
Material::~Material() {}

bool Material::RegisterTexture(string TexturePath, string Name) {
	Texture2D* newTex = new Texture2D(TexturePath);
	if (RegisterTexture(newTex, Name)) {
		return true;
	}
	delete newTex;
	return false;
}
bool Material::RegisterTexture(Texture2D* Texture, string Name) {
	for (int i = 0; i < S_Textures.size(); i++) {
		if (S_Textures[i].Name == Name) {
			return false;
		}
	}
	MaterialTexture newTexture;
	newTexture.Texture = Texture;
	newTexture.Name = Name;
	S_Textures.push_back(newTexture);
	return true;
}
void Material::BindMaterial() {
	S_Shader->Bind();
	for (int i = 0; i < this->S_Textures.size(); i++) {
		glUniform1i(glGetUniformLocation(S_Shader->GetProgram(), S_Textures[i].Name.c_str()), i);
		S_Textures[i].Texture->Bind(i);
	}
}