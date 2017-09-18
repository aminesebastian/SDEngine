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
const float radius = 0.0002;

float linearizeDepth(float depth);
vec4 blurSSAO(float Size);
vec3 normal_from_depth(float depth, vec2 texcoords);

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
			  float total_strength = 5.0;
  
			  float area = 0.0075;
			  float falloff = 0.000001;

			  vec3 random = normalize( texture(noiseTexture, texCoord0 * noiseScale).xyz );
  
			  float depth = texture(worldPosition, texCoord0).a;
 
			  vec3 position = vec3(texCoord0, depth);
			  vec3 normal = normal_from_depth(depth, texCoord0);
  
			  float radius_depth = radius;
			  float occlusion = 0.0;


			  for(int i=0; i < kernelSize; i++) {
  
				vec3 ray = radius_depth * reflect(Samples[i], random);
				vec3 hemi_ray = position + sign(dot(ray,normal)) * ray;
    
				float occ_depth = texture(worldPosition, clamp(hemi_ray.xy,0.0,1.0)).a;
				float difference = depth - occ_depth;
    
				occlusion += step(falloff, difference) * (1.0-smoothstep(falloff, area, difference));

				LitOutput	= vec4(normal, 0.0);
				return;
			  }
  
			  float ao = 1.0 - total_strength * occlusion * (1.0 / kernelSize);
			  ao = pow(ao, 8);
			  LitOutput = vec4(ao);
			  return;
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
vec3 normal_from_depth(float depth, vec2 texcoords) {
  
  const vec2 offset1 = vec2(0.0,0.001);
  const vec2 offset2 = vec2(0.001,0.0);
  
  float depth1 = texture(worldPosition, texcoords + offset1).a;
  float depth2 = texture(worldPosition, texcoords + offset2).a;
  
  vec3 p1 = vec3(offset1, depth1 - depth);
  vec3 p2 = vec3(offset2, depth2 - depth);
  
  vec3 normal = cross(p1, p2);
  normal.z = -normal.z;
  
  return normalize(normal);
}