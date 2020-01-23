#version 400
                                                                        
in vec2 texCoord0;                                                                       
in mat3 tbnMatrix0;                                                                 
in vec3 worldPos0;  
in vec3 normal0;

uniform sampler2D Sprite;
uniform vec3 TINT = vec3(1.0f, 1.0f, 1.0f);

uniform float NEAR_CLIP;
uniform float FAR_CLIP; 
 
layout (location = 1) out vec4 AlbedoOut;  	
													
void main()	{				
	vec4 textureSample = texture(Sprite, texCoord0);
	if(textureSample.a < 1.0f) {
		discard;
	}

	AlbedoOut.rgb		= texture(Sprite, texCoord0).rgb * TINT, 1.0f;		
	AlbedoOut.a			= 1;			
}
