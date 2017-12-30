#version 400
                                                                        
in vec2 texCoord0;                                                                  

uniform sampler2D worldPosition;
uniform sampler2D albedo;
uniform sampler2D RMAO;
uniform sampler2D normal;
uniform sampler2D texCoord;
uniform sampler2D translucency;
uniform sampler2D HDR;
uniform sampler2D finalComp;
uniform sampler2D depthBuffer;

uniform sampler2D noiseTexture;
uniform sampler2D ssaoRough;

uniform mat4 PROJECTION_MATRIX;
uniform mat4 VIEW_MATRIX;
uniform mat4 INV_VIEW_MATRIX;

layout (location = 0) out vec4 WorldPosOut;   
layout (location = 1) out vec4 AlbedoOut;  
layout (location = 2) out vec4 RMAOOut;	      
layout (location = 3) out vec3 NormalOut;     
layout (location = 4) out vec3 TexCoordOut;	
layout (location = 5) out vec4 Translucency;
layout (location = 6) out vec4 HDROutput;	
layout (location = 7) out vec4 LitOutput;

uniform int PASS;
uniform int SAMPLE_COUNT;

uniform	vec3 Samples[128];

uniform vec2 SCREEN_RES;

const vec2 noiseScale = vec2(1920.0/4.0, 1200.0/4.0);
const float radius = 3.5;
const float bias = 0.005;

vec4 blurSSAO(float Size);

void main()	{	
	WorldPosOut			= texture(worldPosition, texCoord0);	
	AlbedoOut	 		= texture(albedo, texCoord0);			
	TexCoordOut			= texture(texCoord, texCoord0).rgb;
	NormalOut			= texture(normal, texCoord0).rgb;	
	RMAOOut				= texture(RMAO, texCoord0);	
	Translucency		= texture(translucency, texCoord0);
	HDROutput			= texture(HDR, texCoord0);

	if(PASS == 0) { 
		vec3 fragPos	= (VIEW_MATRIX * vec4(texture(worldPosition, texCoord0).xyz, 1.0)).xyz;
		vec3 normal		= (transpose(INV_VIEW_MATRIX) * vec4(texture(normal, texCoord0).xyz, 1.0)).xyz;
		normal			= normalize(normal);
		vec3 randomVec	= normalize(texture(noiseTexture, texCoord0 * noiseScale).xyz);

		vec3 tangent	= normalize(randomVec - normal * dot(randomVec, normal));
		vec3 bitangent	= cross(tangent, normal);
		mat3 TBN		= mat3(tangent, bitangent, normal);

		float occlusion = 0.0;
		float matID		= clamp(1 - texture(albedo, texCoord0).a, 0.0, 1.0);

		for (int i = 0; i < SAMPLE_COUNT; i++) {
			vec3 kernelSample	= TBN * Samples[i];
			kernelSample		= fragPos + (kernelSample * radius);

			vec4 offset			= vec4(kernelSample, 1.0);
			offset				= PROJECTION_MATRIX *  offset;
			offset.xyz			/= offset.w;
			offset.xyz			= offset.xyz * 0.5 + 0.5;

			float sampleDepth	= (VIEW_MATRIX*vec4(texture(worldPosition, offset.xy).xyz, 1.0)).z;

			float neighborMatID	= clamp(1 - texture(albedo, offset.xy).a, 0.0, 1.0);
			float rangeCheck	= smoothstep(0.0, 1.0, (radius/5) / abs(fragPos.z - sampleDepth));
			occlusion			+= step(kernelSample.z+bias, sampleDepth) * rangeCheck * matID * neighborMatID;

		}

		occlusion				= 1.0-(occlusion / SAMPLE_COUNT);
		WorldPosOut				= vec4(pow(occlusion, 4));
	}else {
		LitOutput			= blurSSAO(1.0) * texture(finalComp, texCoord0);
	}
}
vec4 blurSSAO(float Size) {
    vec2 texelSize = 1.0 / vec2(textureSize(ssaoRough, 0));
    float result = 0.0;
    for (int x = -2; x < 2; ++x)  {
        for (int y = -2; y < 2; ++y)  {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(ssaoRough, texCoord0 + Size*offset).r;
        }
    }
	return vec4(result / (4.0 * 4.0));
}
