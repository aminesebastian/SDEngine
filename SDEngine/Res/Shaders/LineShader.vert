#version 400                                                                        
                                                                                    
layout (location = 0) in vec3 position;                                                                                         

uniform mat4 MVP;
                                                                    
out vec3 worldPos0;    

void main() {       
    gl_Position			= (MVP) * vec4(position, 1.0);
                
	worldPos0			= (MODEL_MATRIX * vec4(position, 1.0)).xyz;
}