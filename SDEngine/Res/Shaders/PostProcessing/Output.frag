#version 400
                                                                        
in vec2 texCoord0;                                                                  

uniform sampler2D worldPosition;
uniform sampler2D albedo;
uniform sampler2D RMAO;
uniform sampler2D normal;
uniform sampler2D texCoord;
uniform sampler2D matID;
uniform sampler2D HDR;
uniform sampler2D finalComp;
	
out vec4 fragColor;
				
void main()	{	
	fragColor = texture(finalComp, texCoord0);
}
