#include "Transform.h"


Transform::~Transform() {}

void Transform::SetUniformScale(float ScaleIn) {
	Scale.x = ScaleIn;
	Scale.y = ScaleIn;
	Scale.z = ScaleIn;
}
void Transform::SetLocation(vec3 LocationIn) {
	Location = LocationIn;
}
void Transform::SetRotation(vec3 RotationIn) {
	SetRotation(RotationIn.x, RotationIn.y, RotationIn.z);
}
void Transform::SetScale(vec3 ScaleIn) {
	Scale = ScaleIn;
}
void Transform::AddLocation(vec3 LocationIn) {
	Location += LocationIn;
}
void Transform::AddRotation(vec3 RotationIn) {
	Rotation += RotationIn;
}
void Transform::AddScale(vec3 ScaleIn) {
	Scale += ScaleIn;
}