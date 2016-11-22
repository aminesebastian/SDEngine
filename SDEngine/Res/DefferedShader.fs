#version 330
                                                                        
in vec2 texCoord0;                                                                       
in mat3 tbnMatrix0;                                                                 
in vec3 worldPos0;   

uniform sampler2D tex_albedo;
uniform sampler2D tex_RMAO;
uniform sampler2D tex_Normal;
uniform sampler2D tex_emissive;

layout (location = 0) out vec3 WorldPosOut;   
layout (location = 1) out vec4 AlbedoOut;  
layout (location = 3) out vec4 RoughnessOut;	   
layout (location = 2) out vec4 MetalnessOut;     
layout (location = 4) out vec4 EmissiveOut;	
layout (location = 5) out vec4 AOOut;	
layout (location = 6) out vec3 NormalOut;     
layout (location = 7) out vec3 TexCoordOut;	
							
								
void main()	{											
	WorldPosOut     = worldPos0;					
	AlbedoOut       = texture(tex_albedo, texCoord0);				
	TexCoordOut     = vec3(texCoord0, 0.0);	
	NormalOut       = normalize(tbnMatrix0 * ((255.0/128.0) * texture(tex_Normal, texCoord0).xyz)-1);	
	EmissiveOut		= texture(tex_albedo, texCoord0);	
	RoughnessOut    = vec4(texture(tex_RMAO, texCoord0).r);	
	MetalnessOut    = vec4(texture(tex_RMAO, texCoord0).g);									
	AOOut			= vec4(texture(tex_RMAO, texCoord0).b);		
	
}
