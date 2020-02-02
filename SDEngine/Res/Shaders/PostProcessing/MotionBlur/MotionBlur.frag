#version 400
                                                                        
in vec2 texCoord0;                                                                  

const float TARGET_FPS = 60.0f;

uniform sampler2D Source;
uniform sampler2D MotionVector;

uniform int SAMPLES = 20;
uniform float FPS = 60.0f;
uniform float MOTION_BLUR_SCALE	= 10.0;

layout (location = 0) out vec4 Result;   

vec4 ApplyMotionBlur();

void main()	{	
	Result				  = ApplyMotionBlur();
}
vec4 ApplyMotionBlur() {
	vec2 motionVector	= texture(MotionVector, texCoord0).xy * MOTION_BLUR_SCALE * (FPS / TARGET_FPS);
	vec2 pixelSize		= 1.0f/textureSize(Source, 0);
	vec2 stepSize		= pixelSize * motionVector;
	vec4 result			= texture(Source, texCoord0);

	for (int i = 1; i < SAMPLES; ++i) {
		vec2 offset = stepSize * (float(i) / float(SAMPLES - 1) - 0.5);
		result += texture(Source, texCoord0 + offset);
	}
 
	return result /= float(SAMPLES);
}