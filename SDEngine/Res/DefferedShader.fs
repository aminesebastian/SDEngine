#version 330
                                                                        
in vec2 texCoord0;                                                                  
in vec3 normal0;                                                                    
in vec3 worldPos0;    
uniform sampler2D albedo;    
	
layout (location = 0) out vec3 WorldPosOut;   
layout (location = 1) out vec3 AlbedoOut;     
layout (location = 2) out vec3 NormalOut;     
layout (location = 3) out vec3 TexCoordOut;
layout (location = 6) out vec3 CompositeOut;	
																			
void main()	{											
	WorldPosOut     = worldPos0;					
	NormalOut       = normalize(normal0);					
	TexCoordOut     = vec3(texCoord0, 0.0);	
	AlbedoOut       = texture2D(albedo, texCoord0).xyz;		
}
