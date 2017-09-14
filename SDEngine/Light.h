#pragma once
#include "Entity.h"
#include "StaticMesh.h"

enum ELightType {
	POINT,
	DIRECTIONAL
};
struct FLightInfo {
	float Intensity;
	vec3 Color;
	float Attenuation;
	ELightType Type = POINT;
};


class Light : public Entity{
public:
	Light(const Transform IntialTransform, ELightType Type = POINT, float Intensity = 20, vec3 Color = vec3(1, 1, 1), float Attenuation = 25);
	~Light();

	FLightInfo& GetLightInfo() { return S_LightInfo; }
	void SetLightColor(const vec3 Color) { S_LightInfo.Color = Color; }
	void SetLightIntensity(const float Intensity) { S_LightInfo.Intensity = Intensity; }
	void SendShaderInformation(Shader* shader, int index);
	virtual void Draw(Camera* Camera) override;
	virtual void Tick(float DeltaTime) override;
	bool GetDebugMode() { return S_DebugLight; }
	void ToggleDebug(bool bDebug) { S_DebugLight = bDebug; }
private:
	FLightInfo S_LightInfo;
	StaticMesh* S_Probe;
	bool S_DebugLight;
	Material* S_DebugMaterial;
};

