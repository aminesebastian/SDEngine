#version 400
                                                                                                                                  
uniform float NEAR_CLIP;
uniform float FAR_CLIP; 

float linearizeDepth(float depth);
														
void main()	{
	float depth	 = gl_FragCoord.z;
	float dx	 = dFdx(depth);
	float dy	 = dFdy(depth);
	float moment2 = depth * depth + 0.25 * (dx * dx + dy * dy);
	gl_FragColor = vec4(gl_FragCoord.z, moment2, 0.0, 0.0);	
}
float linearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0;
    return (2.0 * NEAR_CLIP * FAR_CLIP) / (FAR_CLIP + NEAR_CLIP - z * (FAR_CLIP - NEAR_CLIP));	
}
