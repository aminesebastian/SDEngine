#pragma once
#include "Rendering/RenderTarget.h"

class GBuffer : public RenderTarget {
public:

	enum GBUFFER_TEXTURE_TYPE {
		GBUFFER_TEXTURE_TYPE_POSITION, //RGB Position, A Depth 
		GBUFFER_TEXTURE_TYPE_ALBEDO,   //RGB Albedo, A MAT_ID
		GBUFFER_TEXTURE_TYPE_RMAO,     //RGB RMAO, A Emissive
		GBUFFER_TEXTURE_TYPE_NORMAL,   //RGB Normal, A EMPTY
		GBUFFER_TEXTURE_TYPE_MOTION,   //RG MotionPerPixel, BA EMPTY
		GBUFFER_NUM_TEXTURES
	};

	GBuffer(vec2 TargetResolution);
	~GBuffer();

private:

};
