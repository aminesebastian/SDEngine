#version 400
                                                                        
in vec2 texCoord0;                                                                  
  
uniform vec3 viewPos; 

uniform sampler2D worldPosition;
uniform sampler2D albedo;
uniform sampler2D roughness;
uniform sampler2D metalness;
uniform sampler2D emissive;
uniform sampler2D AO;
uniform sampler2D normal;
uniform sampler2D texCoord;

out vec4 fragColor;
								
void main()	{															
	fragColor	=	texture(albedo, texCoord0);
	//vec4(texCoord0, 0, 1);	
}
