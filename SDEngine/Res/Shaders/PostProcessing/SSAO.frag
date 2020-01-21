#version 400
                                                                        
in vec2 texCoord0;                                                                  

uniform sampler2D Albedo;
uniform sampler2D WorldPosition;
uniform sampler2D WorldNormal;

uniform sampler2D Input;
uniform sampler2D SSAO;

uniform sampler2D noiseTexture;

uniform mat4 PROJECTION_MATRIX;
uniform mat4 VIEW_MATRIX;
uniform mat4 TRANS_INV_VIEW_MATRIX;

layout (location = 0) out vec4 Result;   

uniform int PASS;
uniform int SAMPLE_COUNT;
uniform	vec3 Samples[256];

uniform vec2 NOISE_SCALE = vec2(1920.0/4.0, 1200.0/4.0);
uniform int BLUR_RADIUS  = 2;
uniform float RADIUS	 = 5.0;
uniform float BIAS		 = 0.005;
uniform float POWER		 = 5.0;

vec4 BlurSSAO(float Size);
vec4 CalculateSSAO();
void main()	{	
	if(PASS == 0) {
		Result = CalculateSSAO();
	}else {
		Result = BlurSSAO(1.0) * texture(Input, texCoord0);
	}
}
vec4 CalculateSSAO() {
	vec3 fragPos	= (VIEW_MATRIX * vec4(texture(WorldPosition, texCoord0).xyz, 1.0)).xyz;
	vec3 normal		= (TRANS_INV_VIEW_MATRIX * vec4(texture(WorldNormal, texCoord0).xyz, 1.0)).xyz;
	normal			= normalize(normal);
	vec3 randomVec	= texture(noiseTexture, texCoord0 * NOISE_SCALE).xyz;

	vec3 tangent	= normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent	= cross(tangent, normal);
	mat3 TBN		= mat3(tangent, bitangent, normal);

	float occlusion = 0.0;
	float matID		= clamp(1 - texture(Albedo, texCoord0).a, 0.0, 1.0);

	for (int i = 0; i < SAMPLE_COUNT; i++) {
		vec3 kernelSample	= TBN * Samples[i];
		kernelSample		= fragPos + (kernelSample * RADIUS);

		vec4 offset			= vec4(kernelSample, 1.0);
		offset				= PROJECTION_MATRIX *  offset;
		offset.xyz			/= offset.w;
		offset.xyz			= offset.xyz * 0.5 + 0.5;

		float sampleDepth	= (VIEW_MATRIX*vec4(texture(WorldPosition, offset.xy).xyz, 1.0)).z;

		float neighborMatID	= clamp(1 - texture(Albedo, offset.xy).a, 0.0, 1.0);
		float rangeCheck	= smoothstep(0.0, 1.0, (RADIUS/5) / abs(fragPos.z - sampleDepth));
		occlusion			+= step(kernelSample.z+BIAS, sampleDepth) * rangeCheck * matID * neighborMatID;
	}

	occlusion = 1.0-(occlusion / SAMPLE_COUNT);
	return vec4(pow(occlusion, POWER));
}
vec4 BlurSSAO(float Size) {
    vec2 texelSize = 1.0 / vec2(textureSize(SSAO, 0));
    float result = 0.0;
    for (int x = -BLUR_RADIUS; x < BLUR_RADIUS; ++x)  {
        for (int y = -BLUR_RADIUS; y < BLUR_RADIUS; ++y)  {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(SSAO, texCoord0 + Size*offset).r;
        }
    }
	return vec4(result / ((2 * BLUR_RADIUS) * (2*BLUR_RADIUS)));
}
