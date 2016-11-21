#pragma once
#include <GLEW/glew.h>
#include <stdio.h>

class GBuffer {
public:

	enum GBUFFER_TEXTURE_TYPE {
		GBUFFER_TEXTURE_TYPE_POSITION,
		GBUFFER_TEXTURE_TYPE_ALBEDO, 
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

private:

	GLuint S_FBO;
	GLuint S_Textures[GBUFFER_NUM_TEXTURES];
	GLuint S_DepthBuffer;
};
