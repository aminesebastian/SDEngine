#version 440
                                                                        
in vec2 texCoord0;                                                                  

uniform vec3 CAMERA_POS;

uniform sampler2D worldPosition;
uniform sampler2D albedo;
uniform sampler2D RMAO;
uniform sampler2D normal;
uniform sampler2D texCoord;
uniform sampler2D shadowDepth;

uniform vec2 SCREEN_RES;

layout (location = 0) out vec4 WorldPosOut;   
layout (location = 1) out vec4 AlbedoOut;  
layout (location = 2) out vec4 RMAOOut;	      
layout (location = 3) out vec3 NormalOut;     
layout (location = 4) out vec3 TexCoordOut;	
layout (location = 6) out vec4 HDR;
layout (location = 7) out vec4 LitOutput;

vec2 poissonDisk[16] = vec2[]( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), 
   vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), 
   vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), 
   vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.26496911, -0.41893023 ), 
   vec2( 0.79197514, 0.19090188 ), 
   vec2( -0.24188840, 0.99706507 ), 
   vec2( -0.81409955, 0.91437590 ), 
   vec2( 0.19984126, 0.78641367 ), 
   vec2( 0.14383161, -0.14100790 ) 
);

const float PI = 3.14159265359;
const int NUM_LIGHTS = 50;
struct LightInfo {
	float Intensity;
	vec3 Color;
	float Attenuation;
	vec3 Position;
	vec3 Direction;
	int Type;
	int CastsShadow;
	mat4 VPMatrix;
};
uniform int LIGHT_COUNT;
uniform	LightInfo lights[NUM_LIGHTS];

vec3 Luminance();

vec3 PointLight(vec3 P, vec3 N, vec3 lightCentre, float lightRadius, vec3 lightColour, float lightIntensity);
vec3 DirectionalLight(vec3 P, vec3 N, vec3 lightDirection, vec3 lightColour, float lightIntensity);
	
vec3 fresnelSchlick(float cosTheta, vec3 F0);  
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);

float random(vec3 seed, int i);
float GeneratePoissonShadowCoverage(vec4 fragPosWorld, int lightIndex);
float linstep(float low, float high, float value);
float GenerateVarianceShadowCoverage(vec4 fragPosWorld, int lightIndex);

void main()	{	
	WorldPosOut			= texture(worldPosition, texCoord0);	
	AlbedoOut	 		= texture(albedo, texCoord0);			
	TexCoordOut			= texture(texCoord, texCoord0).rgb;
	NormalOut			= texture(normal, texCoord0).rgb;	
	RMAOOut				= texture(RMAO, texCoord0);	

	vec4 lighting		= vec4(Luminance(), 1.0);
	float brightness	= dot(lighting.rgb, vec3(0.2126, 0.7152, 0.0722));

	lighting			= clamp(lighting, 0.0, 1.0);

    HDR					= lighting * max(0.0, brightness-1.0f);
	LitOutput			= lighting;

	//LitOutput			= vec4(GeneratePoissonShadowCoverage(texture(worldPosition, texCoord0), 27));
	//LitOutput			= vec4(GenerateVarianceShadowCoverage(texture(worldPosition, texCoord0), 27));
}
vec3 Luminance() {
	vec3 finalLightIntensity;
	if(texture(albedo, texCoord0).a == 0) {
		finalLightIntensity = texture(albedo, texCoord0).rgb * 0.03f;
		for(int i=0; i<LIGHT_COUNT; i++) {
			if(lights[i].Type == 0) {
				finalLightIntensity += PointLight(texture(worldPosition, texCoord0).xyz, texture(normal, texCoord0).xyz, lights[i].Position, lights[i].Attenuation, lights[i].Color, lights[i].Intensity/125);	
			}else{
				vec3 directionalContrib = DirectionalLight(texture(worldPosition, texCoord0).xyz, texture(normal, texCoord0).xyz, lights[i].Direction, lights[i].Color, lights[i].Intensity);	
				if(lights[i].CastsShadow == 1) {
					directionalContrib *= GenerateVarianceShadowCoverage(texture(worldPosition, texCoord0), i);
				}
				finalLightIntensity += directionalContrib;
			}		
		}
	}else{
		finalLightIntensity = vec3(1);
	}
	return texture(albedo, texCoord0).rgb * finalLightIntensity * texture(RMAO, texCoord0).b;
}
float linstep(float low, float high, float value) {
	return clamp((value-low)/(high-low), 0.0, 1.0);
}
float GenerateVarianceShadowCoverage(vec4 fragPosWorld, int lightIndex) {
	vec4 fragLSPos		= lights[lightIndex].VPMatrix * vec4(fragPosWorld.xyz, 1.0);	
	vec3 NDCfragLSPos	= fragLSPos.xyz / fragLSPos.w;
	NDCfragLSPos		= NDCfragLSPos * 0.5 + 0.5;

	vec2 uvScale		= textureSize(shadowDepth, 0) / SCREEN_RES;

	float fragDepth		= (1+fragLSPos.z)/2.0;

	vec2 moments = vec2(0.0);

	for (int i=0;i<16;i++){
		int index = int(16.0*random(texCoord0.xyy, i))%16;
		moments += texture(shadowDepth, NDCfragLSPos.xy/uvScale + poissonDisk[index]/700.0).xy; 	
	}
	moments /= 16.0;


	float p				= step(fragDepth, moments.x);
	float variance		= max(moments.y - moments.x * moments.x, 0.00008);

	float d				= fragDepth - moments.x;
	float pMax			= linstep(0.2, 1.0, variance / (variance + d*d));

	return min(max(p, pMax), 1.0);
}
float GeneratePoissonShadowCoverage(vec4 fragPosWorld, int lightIndex) {
	float bias = max(0.01 * (1.0 - dot(texture(normal, texCoord0).xyz, lights[lightIndex].Direction)), 0.005);
	vec4 fragLSPos		= lights[lightIndex].VPMatrix * vec4(fragPosWorld.xyz, 1.0);	
	vec3 NDCfragLSPos	= fragLSPos.xyz / fragLSPos.w;
	NDCfragLSPos		= NDCfragLSPos * 0.5 + 0.5;

	vec2 uvScale		= textureSize(shadowDepth, 0) / SCREEN_RES;

	float closestDepth	= texture(shadowDepth, NDCfragLSPos.xy/uvScale).r;
	if(fragLSPos.z > 1.0) {
        return 1.0;
	}
	float fragDepth		= (1+fragLSPos.z)/2.0;

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowDepth, 0);

	for (int i=0;i<16;i++){
		int index = int(16.0*random(texCoord0.xyy, i))%16;
		float pcfDepth = texture(shadowDepth, NDCfragLSPos.xy/uvScale + poissonDisk[index]/1000.0).r; 
		shadow += fragDepth > pcfDepth+bias ? 0.0 : 1.0; 	
	}
	shadow /= 16.0;
	return shadow;
}
float random(vec3 seed, int i){
	vec4 seed4 = vec4(seed,i);
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
	return fract(sin(dot_product) * 43758.5453);
}
vec3 PointLight(vec3 fragmentPosition, vec3 N, vec3 lightCentre, float lightRadius, vec3 lightColour, float lightIntensity) {
    vec3 albedoGamma     = texture(albedo, texCoord0).rgb;// * texture(albedo, texCoord0).rgb;
    vec3 L = normalize(lightCentre - fragmentPosition);
	float distance    = length(lightCentre - fragmentPosition);
    float attenuation = lightRadius / (distance * distance);

    float NdotL = max(dot(N, L), 0.0);

	float roughness = max(texture(RMAO, texCoord0).r, 0.01);
	vec3 eyeDir = normalize(CAMERA_POS - fragmentPosition);


	vec3 radiance = lightColour * attenuation * lightIntensity;  
	vec3 H = normalize(eyeDir + L);

	float NDF = DistributionGGX(N, H, roughness);        
    float G   = GeometrySmith(N, eyeDir, L, roughness);      

	float metallic = texture(RMAO, texCoord0).g;
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
vec3 DirectionalLight(vec3 fragmentPosition, vec3 N, vec3 lightDirection, vec3 lightColour, float lightIntensity) {
    vec3 albedoGamma     = texture(albedo, texCoord0).rgb;// * texture(albedo, texCoord0).rgb;
    vec3 L = -1*normalize(lightDirection);

    float NdotL = max(dot(N, L), 0.0);

	float roughness = max(texture(RMAO, texCoord0).r, 0.01);
	vec3 eyeDir = normalize(CAMERA_POS - fragmentPosition);


	vec3 radiance = lightColour * lightIntensity;  
	vec3 H = normalize(eyeDir + L);

	float NDF = DistributionGGX(N, H, roughness);        
    float G   = GeometrySmith(N, eyeDir, L, roughness);      

	float metallic = texture(RMAO, texCoord0).g;
	vec3 F0 = vec3(0.04);
	F0      = mix(F0, albedoGamma, metallic);
	vec3 F  = fresnelSchlick(max(dot(H, eyeDir), 0.0), F0); 

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