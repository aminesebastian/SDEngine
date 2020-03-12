#include "Camera.h"
#include "Camera.reflected.h"


Camera::Camera(const TString& Name, const FTransform& CurrentTransform, const float& FOV, const Vector2D& Dimensions, const float& NearClip, const float& FarClip) : Actor(Name),
NearClipPlane(NearClip),
FarClipPlane(FarClip),
FieldOfView(FOV),
CachedAspectRatio(Dimensions.x / Dimensions.y) {
	SetTransform(CurrentTransform);
	UpVector = Vector3D(0, 0, 1);
	RenderTargetDimensions = Dimensions;
	UpdateMatricies();
}
Camera::~Camera() {}

const float& Camera::GetNearClipPlane() const {
	return NearClipPlane;
}
const float&  Camera::GetFarClipPlane() const {
	return FarClipPlane;
}

void Camera::SetRenderTargetDimensions(const Vector2D& Dimensions) {
	CachedAspectRatio = Dimensions.x / Dimensions.y;
	RenderTargetDimensions = Dimensions;
	UpdateMatricies();
}
vec2 Camera::GetRenderTargetDimensions() const {
	return RenderTargetDimensions;
}
mat4 Camera::GetProjectionMatrix() const {
	return ProjectionMatrix;
}
mat4 Camera::GetOrthographicMatrix() const {
	return OrthographicMatrix;
}
mat4 Camera::GetViewMatrix() const {
	return lookAt(Transform.GetLocation(), Transform.GetLocation() + Transform.GetForwardVector(), UpVector);
}
mat4 Camera::GetLastFrameViewMatrix() const {
	return lookAt(LastFrameTransform.GetLocation(), LastFrameTransform.GetLocation() + LastFrameTransform.GetForwardVector(), UpVector);
}
float Camera::GetThetaFOV() const {
	return (float)glm::tan(FieldOfView / 2.0);
}
float Camera::GetFOV() const {
	return FieldOfView;
}
float Camera::GetAspect() const {
	return CachedAspectRatio;
}
void Camera::AddOrbit(const float& Y, const float& Z) {
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
void Camera::UpdateMatricies() {
	OrthographicMatrix = ortho(0.0f, RenderTargetDimensions.x, RenderTargetDimensions.y, 0.0f, NearClipPlane, FarClipPlane);
	ProjectionMatrix = perspective(glm::radians(FieldOfView), CachedAspectRatio, NearClipPlane, FarClipPlane);
}
void Camera::OnPropertyUpdated(const FProperty* Property) {
	Actor::OnPropertyUpdated(Property);
	if (Property->GetName() == "FieldOfView") {
		UpdateMatricies();
	}
}