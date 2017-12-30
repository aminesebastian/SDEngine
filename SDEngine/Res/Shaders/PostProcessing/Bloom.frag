#version 400
                                                                        
in vec2 texCoord0;                                                                  

uniform sampler2D finalComp;

uniform sampler2D xBloom;
uniform sampler2D yBloom;

uniform vec3 CAMERA_POS;

layout (location = 0) out vec4 WorldPosOut;   
layout (location = 7) out vec4 LitOutput;

uniform int PASS;

uniform float WEIGHTS[5]		= float[] (0.227027, 0.1945946, 0.1216216, 0.04054, 0.006216);
uniform float BLOOM_SIZE		= 5.0;
uniform float BLOOM_BOOST		= 5.0;
uniform float BLOOM_THRESHOLD	= 0.0;
uniform vec3 TINT				= vec3(1.0, 1.0, 1.0);

const int MIP_MAP_LEVEL			= 3;

vec2 X_TEX_OFFSET;
vec2 Y_TEX_OFFSET;

vec4 BloomY();
vec4 BloomX();

void main()	{	
	X_TEX_OFFSET = BLOOM_SIZE / textureSize(yBloom, 0);
	Y_TEX_OFFSET = BLOOM_SIZE / textureSize(xBloom, 0);

	if(PASS == 0) {
		WorldPosOut			= BloomX();
	}else if(PASS == 1) {
		WorldPosOut			= BloomY();
	}else{
		LitOutput			= texture(yBloom, texCoord0)*vec4(TINT, 1.0)*BLOOM_BOOST + texture(finalComp, texCoord0);
	}
}

vec4 BloomX() {
    vec4 bloom = texture(yBloom, texCoord0) * WEIGHTS[0]; 
    for(int i = 1; i < 5; ++i) {
        bloom += max(textureLod(yBloom, texCoord0 + vec2(X_TEX_OFFSET.x * i, 0.0), MIP_MAP_LEVEL)-BLOOM_THRESHOLD, 0) * WEIGHTS[i];
        bloom += max(textureLod(yBloom, texCoord0 - vec2(X_TEX_OFFSET.x * i, 0.0), MIP_MAP_LEVEL)-BLOOM_THRESHOLD, 0) * WEIGHTS[i];
    }
	return bloom;
}
vec4 BloomY() {
    vec4 bloom = texture(xBloom, texCoord0) * WEIGHTS[0]; 
    for(int i = 1; i < 5; ++i) {
        bloom += max(textureLod(xBloom, texCoord0 + vec2(0.0, Y_TEX_OFFSET.y * i), MIP_MAP_LEVEL)-BLOOM_THRESHOLD, 0) * WEIGHTS[i];
        bloom += max(textureLod(xBloom, texCoord0 - vec2(0.0, Y_TEX_OFFSET.y * i), MIP_MAP_LEVEL)-BLOOM_THRESHOLD, 0) * WEIGHTS[i];
    }
	return bloom;
}
