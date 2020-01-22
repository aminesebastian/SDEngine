#version 400
                                                                        
in vec2 texCoord0;                                                                       
in vec3 normal0;

uniform vec3 COLOR = vec3(1.0f, 0.0f, 0.0f);

layout (location = 0) out vec4 Output;   
							
void main()	{			
	float ambientLight = 0.75f;
	float nDotL		   = dot(normal0, vec3(0.0f, 0.0f, 1.0f));
	nDotL			   = max(0.0f, nDotL) + ambientLight;
	Output		       = vec4(COLOR, 1.0f) * max(0.0f, nDotL);	
}
