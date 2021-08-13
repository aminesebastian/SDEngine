#version 400
                                                                        
in vec2 texCoord0;                                                                       
in mat3 tbnMatrix0;                                                                 
in vec3 worldPos0;  

in vec4 currentScreenPos0;
in vec4 lastScreenPos0;

uniform float NEAR_CLIP;
uniform float FAR_CLIP; 
uniform int	MAT_ID;

uniform sampler2D albedo;
uniform sampler2D RMAO;
uniform sampler2D normal;
uniform sampler2D Emissive;

layout (location = 0) out vec4 WorldPosOut;   
layout (location = 1) out vec4 AlbedoOut;  
layout (location = 2) out vec4 RMAOOut;	      
layout (location = 3) out vec3 NormalOut;     
layout (location = 4) out vec3 Motion;

float linearizeDepth(float depth);
														
void main()	{											
	WorldPosOut.rgb		= worldPos0;	
	WorldPosOut.a		= linearizeDepth(gl_FragCoord.z);	

	AlbedoOut			= texture(albedo, texCoord0);	
	AlbedoOut.a			= MAT_ID;			

	vec3 sampledNormal	= ((255.0/128.0) * texture(normal, texCoord0).xyz)-1;
	NormalOut			= normalize(tbnMatrix0 * sampledNormal);	
	RMAOOut.r			= texture(RMAO, texCoord0).r;
	RMAOOut.g			= 0.0f;									
	RMAOOut.b			= texture(RMAO, texCoord0).b;	

	Motion.rgb			= vec3(lastScreenPos0.xy - currentScreenPos0.xy, 1.0f);  
}
float linearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0;
    return (2.0 * NEAR_CLIP * FAR_CLIP) / (FAR_CLIP + NEAR_CLIP - z * (FAR_CLIP - NEAR_CLIP));	
}
