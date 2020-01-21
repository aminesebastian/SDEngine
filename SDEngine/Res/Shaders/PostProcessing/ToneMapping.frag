#version 400
                                                                        
in vec2 texCoord0;                                                                  

uniform sampler2D Output;
uniform float EXPOSURE = 9.0f;
uniform float WHITE_POINT = 7.0f;

layout (location = 0) out vec4 TonemappedOutput;

vec4 FilmicTonemap(vec4 Input, float Exposure, float Whitepoint);

void main()	{	
	TonemappedOutput = FilmicTonemap(texture(Output, texCoord0), EXPOSURE, WHITE_POINT);
}
vec4 FilmicTonemap(vec4 Input, float Exposure, float Whitepoint) {
    float A = 0.15;
	float B = 0.50;
	float C = 0.10;
	float D = 0.20;
	float E = 0.02;
	float F = 0.30;

	vec4 preWhitepoint = Exposure * Input;
    preWhitepoint	   =  ((preWhitepoint*(A*preWhitepoint+C*B)+D*E)/(preWhitepoint*(A*preWhitepoint+B)+D*F))-E/F;
	preWhitepoint	  /= ((Whitepoint*(A*Whitepoint+C*B)+D*E)/(Whitepoint*(A*Whitepoint+B)+D*F))-E/F;

	return preWhitepoint;
}