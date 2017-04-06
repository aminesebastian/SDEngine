#version 400
                                                                        
in vec2 texCoord0;                                                                  

uniform vec3 CAMERA_POS;

uniform sampler2D worldPosition;
uniform sampler2D albedo;
uniform sampler2D RMAO;
uniform sampler2D normal;
uniform sampler2D texCoord;
uniform sampler2D matID;

layout (location = 0) out vec4 WorldPosOut;   
layout (location = 1) out vec4 AlbedoOut;  
layout (location = 2) out vec4 RMAOOut;	      
layout (location = 3) out vec3 NormalOut;     
layout (location = 4) out vec3 TexCoordOut;	
layout (location = 5) out vec4 MatID;
layout (location = 6) out vec4 HDR;	
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

vec3 PointLight(vec3 P, vec3 N, vec3 lightCentre, float lightRadius, vec3 lightColour, float lightIntensity, float cutoff);
vec3 Luminance();	
					
void main()	{	
	WorldPosOut			= texture(worldPosition, texCoord0);	
	AlbedoOut	 		= texture(albedo, texCoord0);			
	TexCoordOut			= texture(texCoord, texCoord0).rgb;
	NormalOut			= texture(normal, texCoord0).rgb;	
	RMAOOut				= texture(RMAO, texCoord0);	
	MatID				= texture(matID, texCoord0);


	vec4 output		=  texture(albedo, texCoord0) * vec4(Luminance(), 1.0);
	LitOutput			= output;

    float brightness = dot(output.rgb, vec3(0.2126, 0.7152, 0.0722));
    HDR = output * max(0.0, brightness-1.0);
}
vec3 Luminance() {
	vec3 finalLightIntensity;
	if(texture(matID, texCoord0) == 0) {
		//finalLightIntensity = texture(albedo, texCoord0).rgb * 0.0005f;
		for(int i=0; i<NUM_LIGHTS; i++) {
			finalLightIntensity += PointLight(texture(worldPosition, texCoord0).xyz, texture(normal, texCoord0).xyz, lights[i].Position, lights[i].Attenuation, lights[i].Color, lights[i].Intensity/1000, 0.020);	
		}
	}else{
		finalLightIntensity = texture(albedo, texCoord0).rgb;
	}
	return finalLightIntensity;
}
vec3 PointLight(vec3 fragmentPosition, vec3 N, vec3 lightCentre, float lightRadius, vec3 lightColour, float lightIntensity, float cutoff) {
    float r = lightRadius;
    vec3 L = normalize(lightCentre - fragmentPosition);
    float d = max(length(L) - r, 0);
    float denom = d/r + 1;
    float attenuation = 1 / (denom*denom);
     
    attenuation = max((attenuation - cutoff) / (1 - cutoff), 0);
    float LdotN = max(dot(N, L), 0.0);

	float roughness = 0.0;//texture(RMAO, texCoord0).r;
	vec3 eyeDir = normalize(CAMERA_POS - fragmentPosition);
    vec3 halfDir = normalize(L + eyeDir);
    float specAngle = max(dot(halfDir, N), 0.0);
    float specularIntensity = pow(specAngle, 2.0+(512*(1-roughness)));

	float metallic = 0.0f;
	vec3 adjustedSpecular = ((texture(albedo, texCoord0).rgb * metallic) + (lightColour * (1.0-metallic))) * specularIntensity;
    return ((adjustedSpecular + lightIntensity)) * LdotN * attenuation;
}
