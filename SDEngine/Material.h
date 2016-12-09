#pragma once
#include "Shader.h"
#include "Texture2D.h"
#include <string>

class Material {

public:
	Material(const std::string ShaderName, const std::string AlbedoTexture = "", const std::string RMAOTexture = ""
		, const std::string NormalTexture = "");
	Material() {};
	~Material();

	void RegisterTexture(unsigned int Unit);
	void BindMaterial();
	Shader* GetShader() { return S_Shader; }

	enum {
		T_ALBEDO,
		NUM_TEXTURES
	};
private:
	Shader* S_Shader;
	Texture2D* S_Textures[NUM_TEXTURES];
};

