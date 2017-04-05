#pragma once
#include "Shader.h"
#include "Texture2D.h"
#include <string>
#include <vector>
#include <map>
extern "C" {
#include "SLogger.h"
}

using namespace std;

struct MaterialTexture {
	Texture2D* Texture;
	string	   Name;
};

class Material {

public:
	Material(const std::string BaseShaderName);
	Material() {};
	~Material();

	bool RegisterTexture(string TexturePath, string Name);
	void BindMaterial();
	Shader* GetShader() { return S_Shader; }
	bool ApplyShader(Shader* NewShader);

	enum {
		T_ALBEDO,
		T_ROUGHNESS,
		T_METALLIC,
		T_AMBIENT_OCCLUSION,
		NUM_TEXTURES
	} typedef ETextureType;
private:
	Shader* S_Shader;
	vector<MaterialTexture> S_Textures;

	bool RegisterTexture(Texture2D* Texture, string Name);
};

