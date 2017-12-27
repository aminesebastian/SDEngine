#version 400
                                                                                                                                  
uniform float NEAR_CLIP;
uniform float FAR_CLIP; 

layout (location = 0) out vec4 DepthFromLight;   

float linearizeDepth(float depth);
														
void main()	{												
	DepthFromLight.r = gl_FragCoord.z;		
}
float linearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0;
    return (2.0 * NEAR_CLIP * FAR_CLIP) / (FAR_CLIP + NEAR_CLIP - z * (FAR_CLIP - NEAR_CLIP));	
}
