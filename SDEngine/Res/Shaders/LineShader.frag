#version 400
                                                                                                                                    
in vec3 worldPos0;  

uniform float NEAR_CLIP;
uniform float FAR_CLIP; 

uniform vec3 COLOR;

layout (location = 0) out vec4 WorldPosOut;   
layout (location = 1) out vec4 AlbedoOut;  
layout (location = 2) out vec4 RMAOOut;	      
layout (location = 3) out vec3 NormalOut;     
layout (location = 4) out vec3 TexCoordOut;	

float linearizeDepth(float depth);
														
void main()	{											
	WorldPosOut.rgb		= worldPos0;	
	WorldPosOut.a		= linearizeDepth(gl_FragCoord.z);	

	AlbedoOut.rgb		= COLOR;	
	AlbedoOut.a			= 1;			//Unlit Mode

	NormalOut			= vec3(0, 0, 1);	

	RMAOOut.r			= 1.0f;
	RMAOOut.g			= 0.0f; 								
	RMAOOut.b			= 1.0f;	
}
float linearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0;
    return (2.0 * NEAR_CLIP * FAR_CLIP) / (FAR_CLIP + NEAR_CLIP - z * (FAR_CLIP - NEAR_CLIP));	
}
