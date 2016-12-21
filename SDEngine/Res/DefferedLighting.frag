#version 400
                                                                        
in vec2 texCoord0;                                                                  

uniform vec3 cameraPos;

uniform sampler2D worldPosition;
uniform sampler2D albedo;
uniform sampler2D emissive;
uniform sampler2D RMAO;
uniform sampler2D normal;
uniform sampler2D texCoord;

const int NUM_LIGHTS = 65;
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
	if(length(texture(worldPosition, texCoord0).xyz - cameraPos) > 750) {
		fragColor = texture(albedo, texCoord0);
	}else{
		if(texture(RMAO, texCoord0).b == 0) {
			fragColor = texture(albedo, texCoord0) * vec4(Luminance(), 1.0);
		}else{
			fragColor = texture(albedo, texCoord0) * vec4(Luminance(), 1.0) * texture(RMAO, texCoord0).b;
		}
	}
}
vec3 Luminance() {
	vec3 finalLightIntensity = vec3(0.15f,0.15f,0.15f);
	for(int i=0; i<NUM_LIGHTS; i++) {
		finalLightIntensity += PointLight(texture(worldPosition, texCoord0).xyz, texture(normal, texCoord0).xyz, lights[i].Position, lights[i].Attenuation, lights[i].Color, lights[i].Intensity, 0.01f);	
	}
	return finalLightIntensity;
}
vec3 PointLight(vec3 fragmentPosition, vec3 N, vec3 lightCentre, float lightRadius, vec3 lightColour, float lightIntensity, float cutoff) {
    // calculate normalized light vector and distance to sphere light surface
    float r = lightRadius;
    vec3 L = fragmentPosition - lightCentre;
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

	float roughness = (texture(RMAO, texCoord0).r)+1;
	float metalness = texture(RMAO, texCoord0).g;
	vec3 eyeDir = cameraPos - texture(worldPosition, texCoord0).xyz;

	float specularIntensity = 0.0f;
    vec3 halfDir = normalize(L + eyeDir);
    float specAngle = max(dot(halfDir, N), 0.0);
    specularIntensity = pow(specAngle, 32.0)*(roughness);

	//if(abs(length(texture(worldPosition, texCoord0).xyz - lightCentre)) > 5) {
	//	return vec3(0);
	//}
	vec3 specColor = (lightColour * (1-metalness)) + (texture(albedo, texCoord0).xyz*metalness);
    return ((specColor * lightIntensity) + specularIntensity) * LdotN * attenuation;
}


/*
	First, we add an ambient component.
	Then, we calculatie lambertian diffuse LdotN.
	Then, we calculate the specular contribution weighted by the roughness coefficient.
	Then, we calculate the specular color weighted by the metalness coefficient.
		For this, we need to calculate the half angle between the camera's view vector and the light vector.
		Then, we raise this to a power (sometimes called shininess) and in this case, we multiply by the inverse of the roughness coefficient.
	Then, we sum the specular intensity with the product of the specular color and the light intensity.
	Then, we compute the product of lamertian lighting with the previous product, also with the attenuation for the light.
	Then, we add our ambient term from the first step. 
	Finally, we multiply in our Ambient Occlusion from our RMAO map.
*/