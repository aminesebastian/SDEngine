#version 400
                                                                        
in vec2 texCoord0;                                                                  

uniform vec3 CAMERA_POS;

uniform sampler2D worldPosition;
uniform sampler2D albedo;
uniform sampler2D RMAO;
uniform sampler2D normal;
uniform sampler2D texCoord;
uniform sampler2D translucency;
uniform sampler2D HDR;
uniform sampler2D finalComp;

layout (location = 7) out vec4 Output;

uniform float EXPOSURE = 7.0f;
uniform float WHITE_POINT = 7.0f;

vec4 FilmicTonemap(vec4 Input, float Exposure, float Whitepoint);

void main()	{	
	Output = FilmicTonemap(texture(finalComp, texCoord0), EXPOSURE, WHITE_POINT);
}
vec4 FilmicTonemap(vec4 Input, float Exposure, float Whitepoint) {
    float A = 0.15;
	float B = 0.50;
	float C = 0.10;
	float D = 0.20;
	float E = 0.02;
	float F = 0.30;

	vec4 preWhitepoint = Exposure * Input;
    preWhitepoint =  ((preWhitepoint*(A*preWhitepoint+C*B)+D*E)/(preWhitepoint*(A*preWhitepoint+B)+D*F))-E/F;
	preWhitepoint /= ((Whitepoint*(A*Whitepoint+C*B)+D*E)/(Whitepoint*(A*Whitepoint+B)+D*F))-E/F;
	if(texture(albedo, texCoord0).a == 0) {
		return preWhitepoint;
	}else{
		return Input;
	}	
}