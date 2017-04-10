#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtx/transform.hpp>
#include <iostream>
#include "Entity.h"

using namespace glm;

class Camera : public Entity {

public:
	Camera(const Transform Transform, float FOV, float Aspect, float NearClip, float FarClip) 
		: Entity(Transform),
		S_NearClip(NearClip),
		S_FarClip(FarClip ){

		S_ProjectionMatrix = perspective(FOV, Aspect, NearClip, FarClip);
		S_UpVector = vec3(0, 1, 0);
	}
	inline float GetNearClipPlane() { return S_NearClip; }
	inline float GetFarClipPlane() { return S_FarClip; }

	inline mat4 GetProjectionMatrix() const{
		return S_ProjectionMatrix;
	}
	inline mat4 GetViewMatrix() {
		return lookAt(S_Transform.GetPosition(), S_Transform.GetPosition()+ S_Transform.GetForwardVector(), S_UpVector);
	}
	inline void AddOrbit(float X, float Y) {
		if (S_Transform.GetRotation().x + X > radians(-89.999)) {
			S_Transform.GetRotation().x += X;
		}else{
			S_Transform.GetRotation().x = radians(-89.999);
		}
		if (S_Transform.GetRotation().x + X < radians(89.999)) {
			S_Transform.GetRotation().x += X;
		}else{
			S_Transform.GetRotation().x = radians(89.999);
		}
		S_Transform.GetRotation().y += Y;
	}

	virtual ~Camera();
	void UpdateCameraPosition(const vec3 NewPosition) { S_Transform.GetPosition() = NewPosition; }

private:
	mat4 S_ProjectionMatrix;
	vec3 S_UpVector;
	float S_NearClip;
	float S_FarClip;
};

