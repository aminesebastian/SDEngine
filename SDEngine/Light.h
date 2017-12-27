#pragma once
#include "Entity.h"
#include "GLM/glm.hpp"
#include "FrameBufferObject.h"
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
	Light(const Transform IntialTransform, ELightType Type = POINT, float Intensity = 20, vec3 Color = vec3(1, 1, 1), float Attenuation = 25, bool CastShadows = false);
	~Light();

	inline mat4 GetViewMatrix() {
		return glm::lookAt(S_Transform.GetPosition(), S_Transform.GetPosition() + S_Transform.GetForwardVector(), vec3(0, 0, 1));
	}

	FLightInfo& GetLightInfo() { return S_LightInfo; }
	void SetLightColor(const vec3 Color) { S_LightInfo.Color = Color; }
	void SetLightIntensity(const float Intensity) { S_LightInfo.Intensity = Intensity; }
	void SendShaderInformation(Shader* shader, int index);
	virtual void Draw(Camera* Camera) override;
	virtual void Tick(float DeltaTime) override;
	bool GetDebugMode() { return bDebugLight; }
	void ToggleDebug(bool bDebug) { bDebugLight = bDebug; }
	void GenerateShadowTexture();

	bool CastsShadows() { return bCastShadows; }
	void SetCastsShadows(bool CastsShadows) { bCastShadows = CastsShadows; }
private:
	FLightInfo S_LightInfo;
	StaticMesh* S_Probe;
	bool bDebugLight;
	bool bCastShadows;
	Material* S_DebugMaterial;

	mat4 S_ShadowOrthoMatrix;
	mat4 S_ViewMatrix;

	FrameBufferObject* S_ShadowDepthBuffer;
};

