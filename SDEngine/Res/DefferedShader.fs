#version 330
                                                                        
in vec2 texCoord0;                                                                  
in vec3 normal0;                                                                    
in vec3 worldPos0;   
 
uniform sampler2D tex_albedo1;
	
layout (location = 0) out vec3 WorldPosOut;   
layout (location = 1) out vec4 AlbedoOut;     
layout (location = 2) out vec3 NormalOut;     
layout (location = 3) out vec3 TexCoordOut;	
																			
void main()	{											
	WorldPosOut     = worldPos0;					
	NormalOut       = normalize(normal0);					
	TexCoordOut     = vec3(texCoord0, 0.0);	
	AlbedoOut       = texture(tex_albedo1, texCoord0).xyz;		
}
