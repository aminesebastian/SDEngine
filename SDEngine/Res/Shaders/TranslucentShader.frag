#version 440

in vec2 texCoord0;                                                                  

uniform vec3 CAMERA_POS;
uniform float Opacity;

uniform sampler2D worldPosition;
uniform sampler2D albedo;
uniform sampler2D RMAO;
uniform sampler2D normal;
uniform sampler2D texCoord;
uniform sampler2D matID;
uniform sampler2D HDR;
uniform sampler2D finalComp;

layout (location = 0) out vec4 WorldPosOut;   
layout (location = 1) out vec4 AlbedoOut;  
layout (location = 2) out vec4 RMAOOut;	      
layout (location = 3) out vec3 NormalOut;     
layout (location = 4) out vec3 TexCoordOut;	
layout (location = 5) out vec4 MatID;
layout (location = 6) out vec4 HDROutput;	
layout (location = 7) out vec4 LitOutput;


const int NUM_LIGHTS = 100;
struct LightInfo {
	float Intensity;
	vec3 Color;
	float Attenuation;
	vec3 Position;
	vec3 Direction;
};
uniform	LightInfo lights[NUM_LIGHTS];

void main() {
	
	LitOutput = texture(worldPosition, texCoord0);		
}