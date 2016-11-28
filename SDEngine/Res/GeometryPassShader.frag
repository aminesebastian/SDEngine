#version 400
                                                                        
in vec2 texCoord0;                                                                       
in mat3 tbnMatrix0;                                                                 
in vec3 worldPos0;   

uniform sampler2D tex_albedo;
uniform sampler2D tex_RMAO;
uniform sampler2D tex_Normal;
uniform sampler2D tex_emissive;

layout (location = 0) out vec3 WorldPosOut;   
layout (location = 1) out vec4 AlbedoOut;  
layout (location = 2) out vec4 RMAOOut;	      
layout (location = 3) out vec4 EmissiveOut;	
layout (location = 4) out vec3 NormalOut;     
layout (location = 5) out vec3 TexCoordOut;	
														
void main()	{											
	WorldPosOut     = worldPos0;					
	AlbedoOut       = texture(tex_albedo, texCoord0);				
	TexCoordOut     = vec3(texCoord0, 0.0);	
	NormalOut       = normalize(tbnMatrix0 * ((255.0/128.0) * texture(tex_Normal, texCoord0).xyz)-1);	
	EmissiveOut		= texture(tex_albedo, texCoord0);	
	RMAOOut.r		= texture(tex_RMAO, texCoord0).r;	
	RMAOOut.g		= texture(tex_RMAO, texCoord0).g;									
	RMAOOut.b		= texture(tex_RMAO, texCoord0).b;		
	
}
