#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtx/transform.hpp>
#include <iostream>
#include "Core/Objects/Entities/Actor.h"
#include "Core/Engine/Engine.h"

using namespace glm;

class Camera : public Actor {

public:
	Camera(TString Name, const Transform CurrentTransform, float FOV, vec2 Dimensions, float NearClip, float FarClip);
	virtual ~Camera();
	const float& GetNearClipPlane() const;
	const float& GetFarClipPlane() const;

	void SetRenderTargetDimensions(vec2 Dimensions);
	vec2 GetRenderTargetDimensions() const;

	mat4 GetProjectionMatrix() const;
	mat4 GetOrthographicMatrix() const;
	mat4 GetViewMatrix() const;
	mat4 GetLastFrameViewMatrix() const;
	float GetThetaFOV() const;
	float GetFOV() const;
	float GetAspect() const;
	void AddOrbit(float Y, float Z);
	void UpdateCameraPosition(const vec3 NewPosition);

private:
	mat4 S_ProjectionMatrix;
	mat4 S_OrthographicMatrix;
	vec3 S_UpVector;
	vec2 RenderTargetDimensions;
	float S_NearClip;
	float S_FarClip;
	float S_FOV;
	float S_Aspect;
};

