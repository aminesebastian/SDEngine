#version 400
                                                                        
in vec2 texCoord0;                                                                  

uniform vec3 viewPosition;

uniform sampler2D worldPosition;
uniform sampler2D albedo;
uniform sampler2D roughness;
uniform sampler2D metalness;
uniform sampler2D emissive;
uniform sampler2D AO;
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
	float lightIntensity	=	max(dot(normalize(-lights[0].Direction), texture(normal, texCoord0).xyz), 0.0) * lights[0].Intensity * 20 + 0.1f;
	fragColor				=	texture(worldPosition, texCoord0)*lightIntensity;
	//vec4(Lights[0].Position, 1.0);	
}
