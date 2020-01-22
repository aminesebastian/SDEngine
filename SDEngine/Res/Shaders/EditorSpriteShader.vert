#version 400                                                                        
                                                                                    
layout (location = 0) in vec3 position;                                             
layout (location = 1) in vec2 texCoord;                                             

uniform mat4 MVP;

out vec2 texCoord0;

void main() {      
	texCoord0   = texCoord;
	gl_Position	= MVP * vec4(position, 1.0);
}