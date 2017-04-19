#version 440

in vec2 texCoord0;                                                                  
in vec3 normal0;  
in vec3 worldPos0;  
in float depth0;

uniform vec2 SCREEN_RES;
uniform int MAT_ID;

uniform vec3 CAMERA_POS;
uniform float Opacity;
uniform vec3 Albedo;

uniform float NEAR_CLIP;
uniform float FAR_CLIP; 

uniform sampler2D worldPosition;
uniform sampler2D albedo;
uniform sampler2D RMAO;
uniform sampler2D normal;
uniform sampler2D texCoord;
uniform sampler2D translucency;
uniform sampler2D HDR;
uniform sampler2D finalComp;

layout (location = 0) out vec4 WorldPosOut;   
layout (location = 1) out vec4 AlbedoOut;  
layout (location = 2) out vec4 RMAOOut;	      
layout (location = 3) out vec3 NormalOut;     
layout (location = 4) out vec3 TexCoordOut;	
layout (location = 5) out vec4 Translucency;
layout (location = 6) out vec4 HDROutput;	
layout (location = 7) out vec4 LitOutput;

const int NUM_LIGHTS = 127;
struct LightInfo {
	float Intensity;
	vec3 Color;
	float Attenuation;
	vec3 Position;
	vec3 Direction;
};
uniform	LightInfo lights[NUM_LIGHTS];

vec3 Lighting();
float Fresnel(float Exponent, float Min, float Max);
float linearizeDepth(float depth);

void main() {
	WorldPosOut			= texture(worldPosition, texCoord0);	
	AlbedoOut	 		= texture(albedo, texCoord0);	
	AlbedoOut.a			= MAT_ID;
	TexCoordOut			= texture(texCoord, texCoord0).rgb;
	NormalOut			= texture(normal, texCoord0).rgb;	
	RMAOOut				= texture(RMAO, texCoord0);	
	//HDROutput			= texture(HDR, texCoord0);
	LitOutput			= texture(finalComp, texCoord0);

	vec2 screenTexCoord = gl_FragCoord.rg;
	screenTexCoord.r   /= SCREEN_RES.x;
	screenTexCoord.g   /= SCREEN_RES.y;
	vec3 refractionDir	= (normalize(CAMERA_POS - worldPos0) - normalize(normal0))-0.5;
	vec2 refCoord		= refractionDir.rg * 0.06 * Fresnel(5, 0, 1);

	vec4 preDepthTest	= (texture(finalComp, screenTexCoord + refCoord)) + (vec4(Lighting()*Albedo*Fresnel(2.0f, 0.05f, 1.0f), 1.0f));		 
	preDepthTest.a		= 1;
	
	HDROutput			= texture(HDR, screenTexCoord) + clamp(preDepthTest-1, 0, 1);
	Translucency		= (preDepthTest);
}

vec3 Lighting() {
	vec3 luminance = vec3(0);
	for(int i=0; i<NUM_LIGHTS; i++) {
		vec3 L = normalize(worldPos0 - lights[i].Position);
	    float NdotL = max(dot(-normal0, L), 0.0);

		vec3 E = normalize(worldPos0 - CAMERA_POS);
		vec3 R = reflect(-L, normal0);
		float tempRoughness = 1024;
		vec3 specular = pow(max(dot(R, E), 0.0), tempRoughness) * lights[i].Color * lights[i].Intensity;

		luminance += specular + vec3(NdotL)*lights[i].Color*lights[i].Intensity/5000;
	}
	return luminance;
}
float Fresnel(float Exponent, float Min, float Max) {
	return pow(clamp(1-abs(dot(normalize(worldPos0 - CAMERA_POS), normal0)), Min, Max), Exponent);
}
float linearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0;
    return (2.0 * NEAR_CLIP * FAR_CLIP) / (FAR_CLIP + NEAR_CLIP - z * (FAR_CLIP - NEAR_CLIP));	
}