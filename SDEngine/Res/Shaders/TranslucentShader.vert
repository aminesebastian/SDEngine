#version 400                                                                        
                                                                                    
layout (location = 0) in vec3 position;                                             
layout (location = 1) in vec2 texCoord;                                             
layout (location = 2) in vec3 normal;                                               
layout (location = 3) in vec3 tangent;   

uniform mat4 MODEL_MATRIX;
uniform mat4 MVP;
                                     
out vec2 texCoord0;                                                                 
out mat3 tbnMatrix0;                                                                     
out vec3 worldPos0;  
out vec3 normal0;                                                             



void main() {       
    gl_Position			= (MVP) * vec4(position, 1.0);
    texCoord0			= texCoord;                  
	worldPos0			= (MODEL_MATRIX * vec4(position, 1.0)).xyz;

    vec3 tempNormal		= normalize((MODEL_MATRIX *vec4(normal, 0.0)).xyz);   
	vec3 tempTangent	= normalize((MODEL_MATRIX *vec4(tangent, 0.0)).xyz);  	
	tempTangent			= normalize(tempTangent - dot(tempTangent, tempNormal) * tempNormal);
		 
	vec3 tempBiTangent	= cross(tempTangent, tempNormal);
    tbnMatrix0			= mat3(tempTangent, tempBiTangent, tempNormal);
	normal0				= normalize((MODEL_MATRIX *vec4(normal, 0.0)).xyz);  
}

