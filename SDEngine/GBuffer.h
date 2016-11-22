#pragma once
#include <GLEW/glew.h>
#include <stdio.h>
#include <cassert>

class GBuffer {
public:

	enum GBUFFER_TEXTURE_TYPE {
		GBUFFER_TEXTURE_TYPE_POSITION,
		GBUFFER_TEXTURE_TYPE_ALBEDO, 
		GBUFFER_TEXTURE_TYPE_ROUGHNESS,
		GBUFFER_TEXTURE_TYPE_METALLNESS,
		GBUFFER_TEXTURE_TYPE_EMISSIVE,
		GBUFFER_TEXTURE_TYPE_AO,
		GBUFFER_TEXTURE_TYPE_NORMAL,
		GBUFFER_TEXTURE_TYPE_TEXCOORD, 
		GBUFFER_NUM_TEXTURES
	};

	GBuffer();

	~GBuffer();

	bool Init(unsigned int WindowWidth, unsigned int WindowHeight);

	void BindForWriting();
	void BindForReading();
	void SetReadBuffer(GBUFFER_TEXTURE_TYPE TextureType);
	GLuint& GetTexture(unsigned int i) { assert(i < GBUFFER_NUM_TEXTURES);  return S_Textures[i]; }
	GLuint S_Textures[GBUFFER_NUM_TEXTURES];
private:

	GLuint S_FBO;
	GLuint S_DepthBuffer;
};
