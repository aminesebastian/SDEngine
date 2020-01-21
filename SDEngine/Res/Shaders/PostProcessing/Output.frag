#version 440
                                                                     
in vec2 texCoord0;                                                                  

uniform sampler2D Output;
	
out vec4 fragColor;
				
void main()	{	
	fragColor = texture(Output, texCoord0);
}
