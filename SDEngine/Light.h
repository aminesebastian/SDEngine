#pragma once
#include "Entity.h"
#include "StaticMesh.h"

enum class ELightType {
	Point,
	Spot,
	Ambient,
	Directional
};
struct FLightInfo {
	float Intensity;
	vec3 Color;
	float Attenuation;
	ELightType Type = ELightType::Point;
};


class Light : public Entity{
public:
	Light(const Transform IntialTransform, float Intensity = 20, vec3 Color = vec3(1, 1, 1), float Attenuation = 25);
	~Light();

	FLightInfo& GetLightInfo() { return S_LightInfo; }
	void SetLightColor(const vec3 Color) { S_LightInfo.Color = Color; }
	void SetLightIntensity(const float Intensity) { S_LightInfo.Intensity = Intensity; }
	void SendShaderInformation(Shader* shader, int index) {
		shader->SetShaderFloat("lights[" + std::to_string(index) + "].Intensity", GetLightInfo().Intensity);
		shader->SetShaderFloat("lights[" + std::to_string(index) + "].Attenuation", GetLightInfo().Attenuation);
		shader->SetShaderVector3("lights[" + std::to_string(index) + "].Color", GetLightInfo().Color);
		shader->SetShaderVector3("lights[" + std::to_string(index) + "].Position", GetTransform().GetPosition());
		shader->SetShaderVector3("lights[" + std::to_string(index) + "].Direction", GetTransform().GetForwardVector());
	}
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

