#version 440
                                                                        
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

const float PI = 3.14159265359;
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
vec3 fresnelSchlick(float cosTheta, vec3 F0);  
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);

void main()	{	
	WorldPosOut			= texture(worldPosition, texCoord0);	
	AlbedoOut	 		= texture(albedo, texCoord0);			
	TexCoordOut			= texture(texCoord, texCoord0).rgb;
	NormalOut			= texture(normal, texCoord0).rgb;	
	RMAOOut				= texture(RMAO, texCoord0);	
	MatID				= texture(matID, texCoord0);


	vec4 lighting		=  texture(albedo, texCoord0) * vec4(Luminance(), 1.0);
	LitOutput			= lighting;

    float brightness = dot(lighting.rgb, vec3(0.2126, 0.7152, 0.0722));
    HDR = lighting * max(0.0, brightness-1.0);
}
vec3 Luminance() {
	vec3 finalLightIntensity;
	if(texture(matID, texCoord0) == 0) {
		finalLightIntensity = texture(albedo, texCoord0).rgb * 0.03f;
		for(int i=0; i<NUM_LIGHTS; i++) {
			finalLightIntensity += PointLight(texture(worldPosition, texCoord0).xyz, texture(normal, texCoord0).xyz, lights[i].Position, lights[i].Attenuation, lights[i].Color, lights[i].Intensity/1000, 0.020);	
		}
	}else{
		finalLightIntensity = texture(albedo, texCoord0).rgb;
	}
	return finalLightIntensity;
}
vec3 PointLight(vec3 fragmentPosition, vec3 N, vec3 lightCentre, float lightRadius, vec3 lightColour, float lightIntensity, float cutoff) {
    vec3 albedoGamma     = texture(albedo, texCoord0).rgb * texture(albedo, texCoord0).rgb;
    vec3 L = normalize(lightCentre - fragmentPosition);
	float distance    = length(lightCentre - fragmentPosition);
    float attenuation = 500.0 / (distance * distance);
    float NdotL = max(dot(N, L), 0.0);

	float roughness = 0.0f;texture(RMAO, texCoord0).r;
	vec3 eyeDir = normalize(CAMERA_POS - fragmentPosition);


	vec3 radiance     = lightColour * attenuation;  
	vec3 H = normalize(eyeDir + L);

	float NDF = DistributionGGX(N, H, roughness);        
    float G   = GeometrySmith(N, eyeDir, L, roughness);      

	float metallic = 0.0f; texture(RMAO, texCoord0).g;
	vec3 F0 = vec3(0.04);
	F0      = mix(F0, albedoGamma, metallic);
	vec3 F    = fresnelSchlick(max(dot(H, eyeDir), 0.0), F0); 

	vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

	vec3 nominator    = NDF * G * F;
    float denominator = 4 * max(dot(N, eyeDir), 0.0) * max(dot(N, L), 0.0) + 0.001; 
    vec3 brdf = nominator / denominator;

    return (kD * albedoGamma / PI + brdf) * radiance * NdotL; 
}
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return nom / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}