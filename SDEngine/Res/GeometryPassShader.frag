#version 400
                                                                        
in vec2 texCoord0;                                                                       
in mat3 tbnMatrix0;                                                                 
in vec3 worldPos0;  
in vec3 normal0;

uniform vec3 albedo0;
uniform float roughness0;
uniform float metalness0;

uniform int useCustomAlbedo;
uniform int useCustomRoughness;
uniform int useCustomMetalness;

uniform float NEAR_CLIP;
uniform float FAR_CLIP; 
uniform int	MAT_ID;

uniform sampler2D tex_albedo;
uniform sampler2D tex_RMAO;
uniform sampler2D tex_Normal;
uniform sampler2D tex_emissive;

layout (location = 0) out vec4 WorldPosOut;   
layout (location = 1) out vec4 AlbedoOut;  
layout (location = 2) out vec4 RMAOOut;	      
layout (location = 3) out vec3 NormalOut;     
layout (location = 4) out vec3 TexCoordOut;	
layout (location = 5) out vec3 MatIDOut;	

float linearizeDepth(float depth);
														
void main()	{											
	WorldPosOut.rgb		= worldPos0;	
	WorldPosOut.a		= linearizeDepth(gl_FragCoord.z);	
	AlbedoOut			= (texture(tex_albedo, texCoord0) * (1-useCustomAlbedo)) + (useCustomAlbedo*vec4(albedo0, 1));				
	TexCoordOut			= vec3(texCoord0, 0.0);
	vec3 sampledNormal	= ((255.0/128.0) * texture(tex_Normal, texCoord0).xyz)-1;
	NormalOut			= normalize(tbnMatrix0 * sampledNormal);	
	RMAOOut.r			= texture(tex_RMAO, texCoord0).r;	
	RMAOOut.g			= texture(tex_RMAO, texCoord0).g;									
	RMAOOut.b			= texture(tex_RMAO, texCoord0).b;	
	MatIDOut			= vec3(MAT_ID);	
}

float linearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0;
    return (2.0 * NEAR_CLIP * FAR_CLIP) / (FAR_CLIP + NEAR_CLIP - z * (FAR_CLIP - NEAR_CLIP));	
}