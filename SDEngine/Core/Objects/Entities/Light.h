#pragma once
#include "Core/Objects/Entities/Actor.h"
#include "GLM/glm.hpp"
#include "Core/Objects/CoreTypes/RenderTarget.h"
#include "Core/Objects/CoreTypes/StaticMesh.h"
#include "Core/Objects/Entities/Components/EditorSpriteComponent.h"

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

class DefferedCompositor;

using namespace glm;

class Light : public Actor {
public:
	Light(TString Name, const Transform IntialTransform, ELightType Type = POINT, float Intensity = 20, vec3 Color = vec3(1, 1, 1), float Attenuation = 25, bool CastShadows = false);
	~Light();

	FLightInfo& GetLightInfo() { return S_LightInfo; }
	void SetLightColor(const vec3 Color) { S_LightInfo.Color = Color; }
	void SetLightIntensity(const float Intensity) { S_LightInfo.Intensity = Intensity; }
	void SendShaderInformation(Shader* shader, int index);
	void GenerateShadowTexture(DefferedCompositor* Compositor);

	mat4 GetLightViewMatrix();
	mat4 GetLightOrthogonalMatrix();

	//virtual bool TraceAgainstRay(vec3 Origin, vec3 Direction, vec3& HitPoint, float& Distance, ECollisionChannel Channel) override;

	bool CastsShadows() { return bCastShadows; }
	void SetCastsShadows(bool CastsShadows) { bCastShadows = CastsShadows; }

	void BlurTexture(RenderTarget* ReadBuffer, RenderTarget* WriteBuffer);

private:
	FLightInfo S_LightInfo;

	bool bCastShadows;
	Material* S_DebugMaterial;

	mat4 S_ShadowOrthoMatrix;
	mat4 S_ViewMatrix;

	RenderTarget* S_ShadowBuffer;
	RenderTarget* S_ShadowBufferTemp;
};

