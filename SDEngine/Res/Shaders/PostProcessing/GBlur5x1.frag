#version 400
                                                                        
in vec2 texCoord0;                                                                  

uniform sampler2D BloomAccumulation;

uniform vec2 DIRECTION;
uniform float BLUR_SIZE;
uniform float DOWNSAMPLE_FACTOR;
uniform int PASS;

layout (location = 0) out vec4 Result;   

const float WEIGHTS[10]	= float[] (0.38972, 0.34481, 0.23883, 0.12949, 0.05495, 0.01825, 0.00474, 0.00097, 0.00015, 0.00002);

vec4 Bloom(vec2 TexOffset);

void main()	{	
	vec2 pixelSize		  = 1.0f/textureSize(BloomAccumulation, 0);
	vec2 textureOffset	  = pixelSize * BLUR_SIZE * DIRECTION;
	int passOffset		  = PASS;
	textureOffset		 *= (1 / (passOffset*DOWNSAMPLE_FACTOR));
	Result				  = Bloom(textureOffset);
}
vec4 Bloom(vec2 TexOffset) {
    vec4 bloom = texture(BloomAccumulation, texCoord0) * WEIGHTS[0]/PASS; 

    for(int i = 1; i < 10; ++i) {
		bloom += texture(BloomAccumulation, texCoord0 + (TexOffset * i)) * WEIGHTS[i]/PASS;
        bloom += texture(BloomAccumulation, texCoord0 - (TexOffset * i)) * WEIGHTS[i]/PASS;
    }

	return bloom;
}