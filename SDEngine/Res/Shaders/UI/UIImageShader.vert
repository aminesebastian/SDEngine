#version 400                                                                        
                        
uniform mat4 MODEL_MATRIX;

layout (location = 0) in vec3 position;                                             
layout (location = 1) in vec2 texCoord;                                                                                          
                             
out vec2 texCoord0;                                                                                                                         
out vec2 screenPos0;

void main() {   
	vec4 screenPosition  = MODEL_MATRIX * vec4(position, 1.0);
	screenPos0			 = screenPosition.xy;
    gl_Position			 = screenPosition;
    texCoord0			 = texCoord;         
}