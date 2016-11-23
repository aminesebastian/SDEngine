#include "Transform.h"


Transform::~Transform() {}

void Transform::SetUniformScale(float Scale) {
	S_Scale.x = Scale;
	S_Scale.y = Scale;
	S_Scale.z = Scale;
}