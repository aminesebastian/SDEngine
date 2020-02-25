#include "Transform.h"
#include "Transform.reflected.h"

Transform::~Transform() {}

void Transform::SetUniformScale(float ScaleIn) {
	Scale.x = ScaleIn;
	Scale.y = ScaleIn;
	Scale.z = ScaleIn;
}
void Transform::SetLocation(Vector3D LocationIn) {
	Location = LocationIn;
}
void Transform::SetRotation(Vector3D RotationIn) {
	SetRotation(RotationIn.x, RotationIn.y, RotationIn.z);
}
void Transform::SetScale(Vector3D ScaleIn) {
	Scale = ScaleIn;
}
void Transform::AddLocation(Vector3D LocationIn) {
	Location += LocationIn;
}
void Transform::AddRotation(Vector3D RotationIn) {
	Rotation += RotationIn;
}
void Transform::AddScale(Vector3D ScaleIn) {
	Scale += ScaleIn;
}