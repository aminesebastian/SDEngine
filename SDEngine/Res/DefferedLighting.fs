#version 400
                                                                        
in vec2 texCoord0;                                                                  

uniform vec3 viewVector;

uniform sampler2D worldPosition;
uniform sampler2D albedo;
uniform sampler2D emissive;
uniform sampler2D RMAO;
uniform sampler2D normal;
uniform sampler2D texCoord;

const int NUM_LIGHTS = 16;
struct LightInfo {
	float Intensity;
	vec3 Color;
	float Attenuation;
	vec3 Position;
	vec3 Direction;
};
uniform	LightInfo lights[NUM_LIGHTS];
	 	
out vec4 fragColor;
						
void main()	{	
	vec3 lightDirection = vec3(0,1,1);
    // set important material values
    float roughnessValue = texture(RMAO, texCoord0).r; // 0 : smooth, 1: rough
    float F0 = 0.8; // fresnel reflectance at normal incidence
    float k = 0.2; // fraction of diffuse reflection (specular reflection = 1 - k)
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    
    // interpolating normals will change the length of the normal, so renormalize the normal.
    vec3 normal1 = texture(normal, texCoord0).xyz;
    
    // do the lighting calculation for each fragment.
    float NdotL = max(dot(normal1, lightDirection), 0.0);
    
    float specular = 0.0;
    if(NdotL > 0.0)
    {
        vec3 eyeDir = normalize(viewVector);

        // calculate intermediary values
        vec3 halfVector = normalize(lightDirection + eyeDir);
        float NdotH = max(dot(normal1, halfVector), 0.0); 
        float NdotV = max(dot(normal1, eyeDir), 0.0); // note: this could also be NdotL, which is the same value
        float VdotH = max(dot(eyeDir, halfVector), 0.0);
        float mSquared = roughnessValue * roughnessValue;
        
        // geometric attenuation
        float NH2 = 2.0 * NdotH;
        float g1 = (NH2 * NdotV) / VdotH;
        float g2 = (NH2 * NdotL) / VdotH;
        float geoAtt = min(1.0, min(g1, g2));
     
        // roughness (or: microfacet distribution function)
        // beckmann distribution function
        float r1 = 1.0 / ( 4.0 * mSquared * pow(NdotH, 4.0));
        float r2 = (NdotH * NdotH - 1.0) / (mSquared * NdotH * NdotH);
        float roughness = r1 * exp(r2);
        
        // fresnel
        // Schlick approximation
        float fresnel = pow(1.0 - VdotH, 5.0);
        fresnel *= (1.0 - F0);
        fresnel += F0;
        
        specular = (fresnel * geoAtt * roughness) / (NdotV * NdotL * 3.14);
    }
    
    vec3 finalValue = lightColor * 10 * NdotL * (k + specular * (1.0 - k));
    fragColor = texture(albedo, texCoord0) * vec4(finalValue, 1.0);
}
