#version 400
                                                                        
in vec2 texCoord0;                                                                  

uniform sampler2D Bloom;
uniform sampler2D Input;
uniform float BLOOM_WEIGHT;

layout (location = 0) out vec4 Result;   

void main()	{	
	Result = texture(Bloom, texCoord0)*BLOOM_WEIGHT + texture(Input, texCoord0);
}