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

uniform sampler2D xBlur;
uniform sampler2D yBlur;

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
uniform float BLOOM_SIZE		= 10.0;
uniform float BLOOM_BOOST		= 10.0;
uniform float BLOOM_THRESHOLD	= 0.0;
int MIP_MAP_LEVEL			= 1;
uniform vec3 TINT				= vec3(1.0, 1.0, 1.0);

vec4 BlurX(float Size);
vec4 BlurY(float Size);

void main()	{	
	float distance = 1.0;
	float size = mix(0.0, BLOOM_SIZE, distance);

	if(PASS == 0) {
		WorldPosOut			= BlurX(size);
	}else if(PASS == 1) {
		WorldPosOut			= BlurY(size);
	}else{
		WorldPosOut			= texture(worldPosition, texCoord0);	
		AlbedoOut	 		= texture(albedo, texCoord0);			
		TexCoordOut			= texture(texCoord, texCoord0).rgb;
		NormalOut			= texture(normal, texCoord0).rgb;	
		RMAOOut				= texture(RMAO, texCoord0);	
		Translucency		= texture(translucency, texCoord0);

		float blend			= 1.0;
		LitOutput			= mix(texture(finalComp, texCoord0), texture(yBlur, texCoord0)*vec4(TINT, 1.0), blend);
		LitOutput			= texture(worldPosition, texCoord0);
	}
}

vec4 BlurX(float Size) {
    vec2 tex_offset = Size / textureSize(yBlur, 0); 
    vec4 bloom		= texture(yBlur, texCoord0) * WEIGHTS[0]; 

    for(int i = 1; i < 5; ++i) {
        bloom += max(texture(yBlur, texCoord0 + vec2(tex_offset.x * i, 0.0))-BLOOM_THRESHOLD, 0) * WEIGHTS[i];
        bloom += max(texture(yBlur, texCoord0 - vec2(tex_offset.x * i, 0.0))-BLOOM_THRESHOLD, 0) * WEIGHTS[i];
    }
	return bloom * BLOOM_BOOST/10.0;
}
vec4 BlurY(float Size) {
    vec2 tex_offset = Size/ textureSize(xBlur, 0);
    vec4 bloom		= texture(xBlur, texCoord0) * WEIGHTS[0]; 
    for(int i = 1; i < 5; ++i) {
        bloom += textureLod(xBlur, texCoord0 + vec2(0.0, tex_offset.y * i), MIP_MAP_LEVEL) * WEIGHTS[i];
        bloom += textureLod(xBlur, texCoord0 - vec2(0.0, tex_offset.y * i), MIP_MAP_LEVEL) * WEIGHTS[i];
    }
	return bloom;
}
