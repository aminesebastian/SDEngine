#version 400                                                                        
                                                                                    
layout (location = 0) in vec3 position;                                             
layout (location = 1) in vec2 texCoord;                                             
layout (location = 2) in vec3 normal;                                               
layout (location = 3) in vec3 tangent;   

uniform mat4 MODEL_MATRIX;
uniform mat3 NORMAL_MODEL_MATRIX;
uniform mat4 VIEW_MATRIX;
uniform mat4 PROJECTION_MATRIX;

uniform mat4 MVP;
uniform mat4 LAST_MVP;
                                     
out vec2 texCoord0;                                                                 
out mat3 tbnMatrix0;                                                                     
out vec3 worldPos0; 
out vec4 currentScreenPos0;
out vec4 lastScreenPos0;

void main() {      
	currentScreenPos0   = MVP * vec4(position, 1.0);
	lastScreenPos0		= LAST_MVP * vec4(position, 1.0);

    gl_Position			= currentScreenPos0;
    texCoord0			= texCoord;                  
	worldPos0			= (MODEL_MATRIX * vec4(position, 1.0)).xyz;


    vec3 tempNormal		= normalize(NORMAL_MODEL_MATRIX * normal);   
	vec3 tempTangent	= normalize(NORMAL_MODEL_MATRIX * tangent);  	
	tempTangent			= normalize(tempTangent - dot(tempTangent, tempNormal) * tempNormal);
		 
	vec3 tempBiTangent	= cross(tempTangent, tempNormal);
    tbnMatrix0			= mat3(tempTangent, tempBiTangent, tempNormal);  
}