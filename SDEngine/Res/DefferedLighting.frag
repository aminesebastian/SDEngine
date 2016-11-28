#version 400
                                                                        
in vec2 texCoord0;                                                                  

uniform vec3 cameraPos;

uniform sampler2D worldPosition;
uniform sampler2D albedo;
uniform sampler2D emissive;
uniform sampler2D RMAO;
uniform sampler2D normal;
uniform sampler2D texCoord;

const int NUM_LIGHTS = 100;
struct LightInfo {
	float Intensity;
	vec3 Color;
	float Attenuation;
	vec3 Position;
	vec3 Direction;
};
uniform	LightInfo lights[NUM_LIGHTS];
	 	
out vec4 fragColor;

vec3 PointLight(vec3 P, vec3 N, vec3 lightCentre, float lightRadius, vec3 lightColour, float lightIntensity, float cutoff);
vec3 Luminance();	
					
void main()	{	
	vec4 aNormal = texture(normal, texCoord0);
	aNormal.g = -aNormal.g;
    fragColor = texture(albedo, texCoord0) * vec4(Luminance(), 1.0);
}
vec3 Luminance() {
	vec3 finalLightIntensity = vec3(0.05f,0.05f,0.05f);
	for(int i=0; i<NUM_LIGHTS; i++) {
		if(lights[i].Intensity != 0) {
				finalLightIntensity += PointLight(texture(worldPosition, texCoord0).xyz, texture(normal, texCoord0).xyz, lights[i].Position, lights[i].Attenuation, lights[i].Color, lights[i].Intensity, 0.1f);	
		}
	}
	return finalLightIntensity;
}
vec3 PointLight(vec3 fragmentPosition, vec3 N, vec3 lightCentre, float lightRadius, vec3 lightColour, float lightIntensity, float cutoff) {
    // calculate normalized light vector and distance to sphere light surface
    float r = lightRadius;
    vec3 L = lightCentre - fragmentPosition;
    float dist = length(L);
    float d = max(dist - r, 0);
    L /= dist;
    N.y = -N.y;
    // calculate basic attenuation
    float denom = d/r + 1;
    float attenuation = 1 / (denom*denom);
     
    // scale and bias attenuation such that:
    //   attenuation == 0 at extent of max influence
    //   attenuation == 1 when d == 0
    attenuation = (attenuation - cutoff) / (1 - cutoff);
    attenuation = max(attenuation, 0);

    // Specular
    float LdotN = max(dot(-L, N), 0.0);

	float cShininess = (1-texture(RMAO, texCoord0).r) * 16;
	float specularIntensity = 0.0f;
	vec3 eyeDir = cameraPos - texture(worldPosition, texCoord0).xyz;
	if (dot(L, texture(normal, texCoord0).xyz) > 0.0) {
		vec3 halfwayVector = normalize(L + eyeDir);
		float specTmp = max(dot(texture(normal, texCoord0).xyz, halfwayVector), 0.0);
		specularIntensity = pow(specTmp, cShininess);
	}

    return (lightColour * lightIntensity + specularIntensity) * LdotN * attenuation;
}
