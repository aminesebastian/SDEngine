#pragma once
#include "Core/Objects/Entities/Actor.h"
#include "GLM/glm.hpp"
#include "Core/Objects/CoreTypes/RenderTarget.h"
#include "Core/Objects/CoreTypes/StaticMesh.h"
#include "Core/Objects/Entities/Components/BillboardComponent.h"
#include "COre/Reflection/Reflection.h"

enum ELightType {
	POINT,
	DIRECTIONAL
};
class DefferedCompositor;

using namespace glm;

SD_CLASS()
class Light : public Actor {
	SD_CLASS_BODY();
public:
	Light(const TString& Name, const FTransform& IntialTransform, const ELightType& Type = POINT, const float& Intensity = 20, const FColor & Color = FColor(1.0f, 1.0f, 1.0f), const float& Attenuation = 25, const bool& CastShadows = false);
	~Light();

	void SetLightColor(const Vector3D& Color);
	void SetLightIntensity(const float& Intensity);
	void SendShaderInformation(Shader* shader, const uint8& index);
	void GenerateShadowTexture(DefferedCompositor* Compositor);

	Matrix4 GetLightViewMatrix();
	Matrix4 GetLightOrthogonalMatrix();
	void BlurTexture(RenderTarget* ReadBuffer, RenderTarget* WriteBuffer);

protected:
	SD_PROPERTY(Category="Light")
		float Intensity;
	SD_PROPERTY(Category="Light")
		float Attenuation;
	SD_PROPERTY(Category="Light")
		Vector3D Color;
	ELightType Type;
private:
	Matrix4 OrthographicMatrix;
	Matrix4 ViewMatrix;
	RenderTarget* PrimaryShadowBuffer;
	RenderTarget* SecondaryShadowBuffer;
};

