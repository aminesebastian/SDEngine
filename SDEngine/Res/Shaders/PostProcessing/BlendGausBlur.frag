#version 400
                                                                        
in vec2 texCoord0;                                                                  

uniform sampler2D Full;
uniform sampler2D Half;

layout (location = 0) out vec4 Result;   

void main()	{	
	Result = texture(Full, texCoord0) + texture(Half, texCoord0) * 0.1;
}