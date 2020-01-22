#include "Transform.h"


Transform::~Transform() {}

void Transform::SetUniformScale(float Scale) {
	S_Scale.x = Scale;
	S_Scale.y = Scale;
	S_Scale.z = Scale;
}
void Transform::SetLocation(vec3 Location) {
	S_Location = Location;
}
void Transform::SetRotation(vec3 Rotation) {
	SetRotation(Rotation.x, Rotation.y, Rotation.z);
}
void Transform::SetScale(vec3 Scale) {
	S_Scale = Scale;
}