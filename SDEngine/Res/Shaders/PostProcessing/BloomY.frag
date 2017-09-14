#version 400
                                                                        
in vec2 texCoord0;                                                                  

uniform vec3 CAMERA_POS;

uniform sampler2D worldPosition;
uniform sampler2D albedo;
uniform sampler2D RMAO;
uniform sampler2D normal;
uniform sampler2D texCoord;
uniform sampler2D translucency;
uniform sampler2D HDR;
uniform sampler2D finalComp;

layout (location = 0) out vec4 WorldPosOut;   
layout (location = 1) out vec4 AlbedoOut;  
layout (location = 2) out vec4 RMAOOut;	      
layout (location = 3) out vec3 NormalOut;     
layout (location = 4) out vec3 TexCoordOut;	
layout (location = 5) out vec4 Translucency;
layout (location = 6) out vec4 HDROutput;	
layout (location = 7) out vec4 LitOutput;

uniform float weight[10] = float[] (0.227027, 0.2045946, 0.1516216, 0.104054, 0.086216, 0.077027, 0.0545946, 0.0416216, 0.024054, 0.016216);
uniform float BLOOM_SIZE = 10.0;

vec4 Bloom();

void main()	{	
	WorldPosOut			= texture(worldPosition, texCoord0);	
	AlbedoOut	 		= texture(albedo, texCoord0);			
	TexCoordOut			= texture(texCoord, texCoord0).rgb;
	NormalOut			= texture(normal, texCoord0).rgb;	
	RMAOOut				= texture(RMAO, texCoord0);	
	Translucency		= texture(translucency, texCoord0);

	LitOutput			= texture(finalComp, texCoord0) + Bloom();
}
vec4 Bloom() {
    vec2 tex_offset = BLOOM_SIZE/ textureSize(HDR, 0)/8; // gets size of single texel
    vec4 bloom = texture(HDR, texCoord0) * weight[0]; // current fragment's contribution

    for(int i = 1; i < 30; ++i) {
		bloom += texture(HDR, texCoord0 + vec2(0.0, tex_offset.y * i)) * weight[i/3];
        bloom += texture(HDR, texCoord0 - vec2(0.0, tex_offset.y * i)) * weight[i/3];
    }
	return bloom/2.5f;
}