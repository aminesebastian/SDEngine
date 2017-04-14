#pragma once
#include <GLEW/glew.h>
#include <stdio.h>
#include <cassert>
#include "Texture2D.h"
#include <vector>

class GBuffer {
public:

	enum GBUFFER_TEXTURE_TYPE {
		GBUFFER_TEXTURE_TYPE_POSITION,
		GBUFFER_TEXTURE_TYPE_ALBEDO, 
		GBUFFER_TEXTURE_TYPE_RMAO,
		GBUFFER_TEXTURE_TYPE_NORMAL,
		GBUFFER_TEXTURE_TYPE_TEXCOORD,
		GBUFFER_TEXTURE_TYPE_TRANSLUCENCY,
		GBUFFER_TEXTURE_TYPE_HDR,
		GBUFFER_TEXTURE_TYPE_FINAL_COMP,
		GBUFFER_NUM_TEXTURES
	};

	GBuffer();

	~GBuffer();

	bool Init(unsigned int WindowWidth, unsigned int WindowHeight);

	std::string GetGBufferTextureName(int Index) { return S_GBufferTextureNames[Index]; }

	void BindForWriting();
	void BindForReading();
	void SetReadBuffer(GBUFFER_TEXTURE_TYPE TextureType);
	GLuint& GetTexture(unsigned int i) { assert(i < GBUFFER_NUM_TEXTURES);  return S_Textures[i]; }
	GLuint S_Textures[GBUFFER_NUM_TEXTURES];
	GLuint S_FBO;

private:
	GLuint S_DepthBuffer;
	std::vector<std::string> S_GBufferTextureNames;
};
