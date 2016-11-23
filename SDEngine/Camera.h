#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtx/transform.hpp>
#include <iostream>
#include "Transform.h"

using namespace glm;

class Camera {

public:
	Camera(const Transform Transform, float FOV, float Aspect, float NearClip, float FarClip) {
		S_ProjectionMatrix = perspective(FOV, Aspect, NearClip, FarClip);
		S_Transform = Transform;
		S_UpVector = vec3(0, 1, 0);
		//S_Transform.GetRotation().y = -90.0f;
	}
	
	inline mat4 GetProjectionMatrix() const{
		return S_ProjectionMatrix;
	}
	inline mat4 GetViewMatrix() {
		return lookAt(S_Transform.GetPosition(), S_Transform.GetPosition()+ S_Transform.GetForwardVector(), S_UpVector);
	}
	inline void AddOrbit(float X, float Y) {
		S_Transform.GetRotation().y += Y;
		S_Transform.GetRotation().x += X;
	}
	virtual ~Camera();
	Transform& GetCameraTransform() { return S_Transform; }
	void UpdateCameraPosition(const vec3 NewPosition) { S_Transform.GetPosition() = NewPosition; };

private:
	mat4 S_ProjectionMatrix;
	Transform S_Transform;
	vec3 S_UpVector;
};

