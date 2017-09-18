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
uniform mat4 INV_PROJECTION_MATRIX;

uniform float NEAR_CLIP;
uniform float FAR_CLIP;

uniform vec3 CAMERA_POS;

layout (location = 0) out vec4 WorldPosOut;   
layout (location = 1) out vec4 AlbedoOut;  
layout (location = 2) out vec4 RMAOOut;	      
layout (location = 3) out vec3 NormalOut;     
layout (location = 4) out vec3 TexCoordOut;	
layout (location = 5) out vec4 Translucency;
layout (location = 6) out vec4 HDROutput;	
layout (location = 7) out vec4 LitOutput;

uniform int PASS;

uniform float THETA_FOV;
uniform float ASPECT;

uniform	vec3 Samples[64];

const vec2 noiseScale = vec2(3000.0/4.0, 1200.0/4.0);
const int kernelSize = 64;
const float radius = 0.00002;

float linearizeDepth(float depth);
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
		if(texture(albedo, texCoord0).a == 1) {
			//WorldPosOut		=  vec4(1.0);
		}else{
			vec2 ndc		= texCoord0 * 2.0 - 1.0;
			vec3 vViewRay	= vec3(ndc.x * THETA_FOV * ASPECT, ndc.y * THETA_FOV, 1.0);
			vec3 origin		= vViewRay* texture(worldPosition, texCoord0).a;
			vec3 normalVec	= (VIEW_MATRIX * texture(normal, texCoord0)).xyz * 2.0 - 1.0;
			normalVec		= normalize(normalVec);
			vec3 randomVec	= normalize(texture(noiseTexture, texCoord0 * noiseScale).xyz);

			vec3 tangent	= normalize(randomVec - normalVec * dot(randomVec, normalVec));
			vec3 bitangent	= cross(tangent, normalVec);
			mat3 TBN		= mat3(tangent, bitangent, normalVec);

			float occlusion = 0.0;
			for (int i = 0; i < kernelSize; ++i) {
				vec3 kernelSample	= TBN * Samples[i];
				kernelSample		= (kernelSample * radius) + origin;
  
				vec4 offset			= vec4(kernelSample, 1.0);
				offset				= PROJECTION_MATRIX * offset;
				offset.xyz			/= -offset.w;
				offset.xy			= offset.xy * 0.5 + 0.5;
				offset.z			= 0;

				float sampleDepth	= texture(worldPosition, offset.xy).a;
  
				float rangeCheck	= abs(origin.z - sampleDepth) < radius ? 1.0 : 0.0;
				occlusion			+= (sampleDepth >= kernelSample.z ? 1.0 : 0.0) * rangeCheck;

				LitOutput			= vec4(texture(worldPosition, texCoord0).a);
				return;
			}
			occlusion				= 1.0-(occlusion / kernelSize);
			WorldPosOut				= vec4(pow(occlusion, 4));
			LitOutput				= vec4(pow(occlusion, 4));
		}
	}else {
		LitOutput			= blurSSAO(2.0) * texture(finalComp, texCoord0) * 10;
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
float linearizeDepth(float depth){
	float n = NEAR_CLIP; // camera z near
	float f = FAR_CLIP; // camera z far
	return (2.0 * n) / (f + n - depth * (f - n));
}