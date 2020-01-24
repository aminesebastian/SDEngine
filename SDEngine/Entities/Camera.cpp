#include "Camera.h"


Camera::Camera(TString Name, const Transform CurrentTransform, float FOV, vec2 Dimensions, float NearClip, float FarClip) : Actor(Name),
	S_NearClip(NearClip),
	S_FarClip(FarClip),
	S_FOV(FOV),
	S_Aspect(Dimensions.x/Dimensions.y) {
	SetTransform(CurrentTransform);
	S_OrthographicMatrix = ortho(0.0f, Dimensions.x, Dimensions.y, 0.0f, NearClip, FarClip);
	S_ProjectionMatrix = perspective(FOV, S_Aspect, NearClip, FarClip);
	S_UpVector = vec3(0, 0, 1);
	RenderTargetDimensions = Dimensions;
}
Camera::~Camera() {}

float Camera::GetNearClipPlane() { return S_NearClip; }
float Camera::GetFarClipPlane() { return S_FarClip; }

void Camera::SetRenderTargetDimensions(vec2 Dimensions) {
	S_Aspect = Dimensions.x / Dimensions.y;
	S_OrthographicMatrix = ortho(0.0f, Dimensions.x, Dimensions.y, 0.0f, S_NearClip, S_FarClip);
	S_ProjectionMatrix = perspective(S_FOV, S_Aspect, S_NearClip, S_FarClip);
	RenderTargetDimensions = Dimensions;
}
vec2 Camera::GetRenderTargetDimensions() {
	return RenderTargetDimensions;
}
mat4 Camera::GetProjectionMatrix() const {
	return S_ProjectionMatrix;
}
mat4 Camera::GetOrthographicMatrix() const {
	return S_OrthographicMatrix;
}
mat4 Camera::GetViewMatrix() {
	return lookAt(CurrentTransform.GetLocation(), CurrentTransform.GetLocation() + CurrentTransform.GetForwardVector(), S_UpVector);
}
mat4 Camera::GetLastFrameViewMatrix() {
	return lookAt(LastFrameTrasnform.GetLocation(), LastFrameTrasnform.GetLocation() + LastFrameTrasnform.GetForwardVector(), S_UpVector);
}
float Camera::GetThetaFOV() {
	return (float)glm::tan(S_FOV / 2.0);
}
float Camera::GetFOV() {
	return S_FOV;
}
float Camera::GetAspect() {
	return S_Aspect;
}
void Camera::AddOrbit(float Y, float Z) {
	if (GetRotation().y + Y > radians<float>(-89.999f)) {
		AddRotation(0.0f, Y, 0.0f);
	} else {
		SetRotation(GetTransform().GetRotation().x, radians<float>(-89.999f), GetTransform().GetRotation().z);;
	}
	if (GetRotation().y + Y < radians<float>(89.999f)) {
		AddRotation(0.0f, Y, 0.0f);
	} else {
		SetRotation(GetTransform().GetRotation().x, radians<float>(89.999f), GetTransform().GetRotation().z);
	}
	AddRotation(0.0f, 0.0f, Z);
}
void Camera::UpdateCameraPosition(const vec3 NewPosition) { CurrentTransform.GetLocation() = NewPosition; }