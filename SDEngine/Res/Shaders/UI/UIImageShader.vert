#version 400                                                                        
                        
uniform vec2 NDC_LOCATION;
uniform vec2 RELATIVE_SIZE;

layout (location = 0) in vec3 position;                                             
layout (location = 1) in vec2 texCoord;     

out vec2 screenPos0;
out vec2 texCoord0;       

void main() {   
	vec3 screenPosition  = vec3(RELATIVE_SIZE * 2.0f, 0.0f) * position;
	screenPosition		+= vec3(NDC_LOCATION, 0.0f);
	screenPos0			 = screenPosition.xy;
	texCoord0			 = texCoord;  
    gl_Position			 = vec4(screenPosition, 1.0f);     
}