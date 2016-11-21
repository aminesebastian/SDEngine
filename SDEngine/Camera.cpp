#include "Camera.h"


Camera::~Camera() {
}
void Camera::UpdateCameraPosition(const vec3& NewPosition) { S_Position = NewPosition; }