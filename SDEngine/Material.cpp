#include "Material.h"



Material::Material(const std::string ShaderName, const std::string AlbedoTexture) {

	S_Textures[0] = &Texture2D(AlbedoTexture);
	//S_Textures[1] = &Texture2D(RoughessTexture);
	//S_Textures[2] = &Texture2D(MetalnessTexture);


	S_Shader = &Shader(ShaderName);
}
Material::~Material() {}

void Material::RegisterTexture(unsigned int Unit) {

}
void Material::BindMaterial() {
	S_Shader->Bind();
	S_Textures[0]->Bind(0);
}