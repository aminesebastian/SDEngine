#version 400
                                                                        
in vec2 texCoord0;                                                                  

uniform sampler2D worldPosition;
uniform sampler2D albedo;
uniform sampler2D RMAO;
uniform sampler2D normal;
uniform sampler2D texCoord;
uniform sampler2D translucency;
uniform sampler2D HDR;
uniform sampler2D finalComp;

uniform sampler2D xBloom;
uniform sampler2D yBloom;

uniform vec3 CAMERA_POS;

layout (location = 0) out vec4 WorldPosOut;   
layout (location = 1) out vec4 AlbedoOut;  
layout (location = 2) out vec4 RMAOOut;	      
layout (location = 3) out vec3 NormalOut;     
layout (location = 4) out vec3 TexCoordOut;	
layout (location = 5) out vec4 Translucency;
layout (location = 6) out vec4 HDROutput;	
layout (location = 7) out vec4 LitOutput;

uniform int PASS;

uniform float WEIGHTS[5]		= float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
uniform float BLOOM_SIZE		= 8.0;
uniform float BLOOM_BOOST		= 10.0;
uniform float BLOOM_THRESHOLD	= 0.0;
const int MIP_MAP_LEVEL			= 3;
uniform vec3 TINT				= vec3(1.0, 1.0, 1.0);

vec4 BloomY(float Size);
vec4 BloomX(float Size);

void main()	{	
	float size = BLOOM_SIZE;

	if(PASS == 0) {
		WorldPosOut			= BloomX(size);
	}else if(PASS == 1) {
		WorldPosOut			= BloomY(size);
	}else{
		WorldPosOut			= texture(worldPosition, texCoord0);	
		AlbedoOut	 		= texture(albedo, texCoord0);			
		TexCoordOut			= texture(texCoord, texCoord0).rgb;
		NormalOut			= texture(normal, texCoord0).rgb;	
		RMAOOut				= texture(RMAO, texCoord0);	
		Translucency		= texture(translucency, texCoord0);

		LitOutput			= texture(yBloom, texCoord0)*vec4(TINT, 1.0)*BLOOM_BOOST + texture(finalComp, texCoord0);
	}
}

vec4 BloomX(float Size) {
    vec2 tex_offset = Size / textureSize(yBloom, 0); 
    vec4 bloom		= texture(yBloom, texCoord0) * WEIGHTS[0]; 

    for(int i = 1; i < 5; ++i) {
        bloom += max(texture(yBloom, texCoord0 + vec2(tex_offset.x * i, 0.0))-BLOOM_THRESHOLD, 0) * WEIGHTS[i];
        bloom += max(texture(yBloom, texCoord0 - vec2(tex_offset.x * i, 0.0))-BLOOM_THRESHOLD, 0) * WEIGHTS[i];
    }
	return bloom * BLOOM_BOOST/10.0;
}
vec4 BloomY(float Size) {
    vec2 tex_offset = Size/ textureSize(xBloom, 0);
    vec4 bloom		= texture(xBloom, texCoord0) * WEIGHTS[0]; 
    for(int i = 1; i < 5; ++i) {
        bloom += textureLod(xBloom, texCoord0 + vec2(0.0, tex_offset.y * i), MIP_MAP_LEVEL) * WEIGHTS[i];
        bloom += textureLod(xBloom, texCoord0 - vec2(0.0, tex_offset.y * i), MIP_MAP_LEVEL) * WEIGHTS[i];
    }
	return bloom;
}
