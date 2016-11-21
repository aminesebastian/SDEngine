#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtx/transform.hpp>

using namespace glm;

class Camera {

public:
	Camera(const vec3& Position, float FOV, float Aspect, float NearClip, float FarClip) {
		S_Perspective = perspective(FOV, Aspect, NearClip, FarClip);
		S_Position = Position;
		S_ForwardVector = vec3(0, 0, 1);
		S_UpVector = vec3(0, 1, 0);
	}
	
	inline mat4 GetViewProjection() const{
		return S_Perspective * lookAt(S_Position, S_Position + S_ForwardVector, S_UpVector);
	}

	virtual ~Camera();
	vec3& GetCameraPosition() { return S_Position; }
	void UpdateCameraPosition(const vec3& NewPosition);

private:
	mat4 S_Perspective;
	vec3 S_Position;
	vec3 S_ForwardVector;
	vec3 S_UpVector;
};

