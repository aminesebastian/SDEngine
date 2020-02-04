#version 400                                                                        
                                                                                    
layout (location = 0) in vec2 position;                                             
layout (location = 1) in vec2 texCoord;                                             

uniform vec2 LOCATION;
uniform vec2 SCALE;

out vec2 texCoord0;

void main() {      
	texCoord0           = texCoord;
	vec2 screenPosition = (position * SCALE) + LOCATION;
	gl_Position	        = vec4(screenPosition, 0.0f, 1.0f);
}