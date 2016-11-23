#pragma once
#include "Transform.h"

struct FLightInfo {
	float Intensity;
	vec3 Color;
	float Attenuation;
};
class Light {
public:
	Light(Transform IntialTransform, float Intensity = 1, vec3 Color = vec3(1,1,1), float Attenuation = 500) :
		S_Transform(IntialTransform) {
		S_LightInfo.Intensity = Intensity;
		S_LightInfo.Color = Color;
		S_LightInfo.Attenuation = Attenuation;
	}
	~Light();

	Transform& GetTransform() { return S_Transform; }
	FLightInfo& GetLightInfo() { return S_LightInfo; }

private:
	Transform S_Transform;
	FLightInfo S_LightInfo;
};

