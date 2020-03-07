#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtx/transform.hpp>
#include <iostream>
#include "Core/Objects/Entities/Actor.h"
#include "Core/Engine/Engine.h"

using namespace glm;

SD_CLASS()
class Camera : public Actor {
	SD_CLASS_BODY()
public:
	Camera(const TString& Name, const FTransform& CurrentTransform, const float& FOV, const Vector2D& Dimensions, const float& NearClip, const float& FarClip);
	virtual ~Camera();

	void SetRenderTargetDimensions(const Vector2D& Dimensions);
	void AddOrbit(const float& Y, const float& Z);

	const float& GetNearClipPlane() const;
	const float& GetFarClipPlane() const;
	Matrix4 GetProjectionMatrix() const;
	Matrix4 GetOrthographicMatrix() const;
	Matrix4 GetViewMatrix() const;
	Matrix4 GetLastFrameViewMatrix() const;
	float GetThetaFOV() const;
	float GetFOV() const;
	float GetAspect() const;
	Vector2D GetRenderTargetDimensions() const;
	void OnPropertyUpdated(FProperty* Property);
protected:
	SD_PROPERTY();
	float FieldOfView;

	void UpdateMatricies();
private:
	Matrix4 ProjectionMatrix;
	Matrix4 OrthographicMatrix;
	Vector3D UpVector;
	Vector2D RenderTargetDimensions;
	float NearClipPlane;
	float FarClipPlane;
	float CachedAspectRatio;
};

