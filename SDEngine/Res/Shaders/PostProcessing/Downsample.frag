#version 400
                                                                        
in vec2 texCoord0;                                                                  

uniform sampler2D Input;

uniform float DOWNSAMPLE_FACTOR;

layout (location = 0) out vec4 Result;   

void main()	{	
	vec2 newCoords = texCoord0;
	Result = texture(Input, newCoords);
}