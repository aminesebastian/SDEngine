#pragma once
#include <GLEW/glew.h>

#define GBUFFER_POSITION_TEXTURE_UNIT 0
#define GBUFFER_ALBEDO_TEXTURE_UNIT  1
#define GBUFFER_NORMAL_TEXTURE_UNIT   2
#define GBUFFER_TEXCOORD_TEXTURE_UNIT 3

class GBuffer {
public:

	enum GBUFFER_TEXTURE_TYPE {
		GBUFFER_TEXTURE_TYPE_POSITION,
		GBUFFER_TEXTURE_TYPE_ALBEDO, 
		GBUFFER_TEXTURE_TYPE_NORMAL,
		GBUFFER_TEXTURE_TYPE_TEXCOORD, 
		GBUFFER_TEXTURE_TYPE_ROUGHNESS,
		GBUFFER_TEXTURE_TYPE_METALNESS,
		GBUFFER_TEXTURE_TYPE_COMPOSITE,
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
