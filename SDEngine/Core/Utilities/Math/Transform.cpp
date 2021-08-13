#include "Transform.h"
#include "Transform.reflected.h"

FTransform::~FTransform() {}

void FTransform::SetUniformScale(float ScaleIn) {
	Scale.x = ScaleIn;
	Scale.y = ScaleIn;
	Scale.z = ScaleIn;
}
void FTransform::SetLocation(Vector3D LocationIn) {
	Location = LocationIn;
}
void FTransform::SetRotation(Vector3D RotationIn) {
	SetRotation(RotationIn.x, RotationIn.y, RotationIn.z);
}
void FTransform::SetScale(Vector3D ScaleIn) {
	Scale = ScaleIn;
}
void FTransform::AddLocation(Vector3D LocationIn) {
	Location += LocationIn;
}
void FTransform::AddRotation(Vector3D RotationIn) {
	Rotation += RotationIn;
}
void FTransform::AddScale(Vector3D ScaleIn) {
	Scale += ScaleIn;
}