#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtx/transform.hpp>
#include <iostream>
#include "Entity.h"
#include "Engine.h"

using namespace glm;

class Camera : public Entity {

public:
	Camera(TString Name, const Transform Transform, float FOV, float Aspect, float NearClip, float FarClip)
		: Entity(Name, Transform),
		S_NearClip(NearClip),
		S_FarClip(FarClip ),
		S_FOV(FOV),
		S_Aspect(Aspect)
	{
		S_OrthographicMatrix = ortho(0.0f, (float)WINDOW_WIDTH, (float)WINDOW_WIDTH, 0.0f, NearClip, FarClip);
		S_ProjectionMatrix = perspective(FOV, Aspect, NearClip, FarClip);
		S_UpVector = vec3(0, 0, 1);
	}
	inline float GetNearClipPlane() { return S_NearClip; }
	inline float GetFarClipPlane() { return S_FarClip; }

	inline mat4 GetProjectionMatrix() const{
		return S_ProjectionMatrix;
	}
	inline mat4 GetOrthographicMatrix() const {
		return S_OrthographicMatrix;
	}
	inline mat4 GetViewMatrix() {
		return lookAt(S_Transform.GetPosition(), S_Transform.GetPosition() + S_Transform.GetForwardVector(), S_UpVector);
	}
	inline float GetThetaFOV() {
		return (float)glm::tan(S_FOV / 2.0);
	}
	inline float GetFOV() {
		return S_FOV;
	}
	inline float GetAspect() {
		return S_Aspect;
	}
	void AddOrbit(float Y, float Z) {
		if (S_Transform.GetRotation().y + Y > radians(-89.999)) {
			S_Transform.GetRotation().y += Y;
		}else{
			S_Transform.GetRotation().y = radians(-89.999);
		}
		if (S_Transform.GetRotation().y + Y < radians(89.999)) {
			S_Transform.GetRotation().y += Y;
		}else{
			S_Transform.GetRotation().y = radians(89.999);
		}
		S_Transform.GetRotation().z += Z;
	}
	virtual ~Camera();
	void UpdateCameraPosition(const vec3 NewPosition) { S_Transform.GetPosition() = NewPosition; }

private:
	mat4 S_ProjectionMatrix;
	mat4 S_OrthographicMatrix;
	vec3 S_UpVector;
	float S_NearClip;
	float S_FarClip;
	float S_FOV;
	float S_Aspect;
};

