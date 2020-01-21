#include "FAABB.h"
#include "Shader.h"
#include "EngineStatics.h"
#include "Camera.h"
#include <limits>
#include "MathLibrary.h"

FAABB::FAABB(SArray<vec3> Verticies) {
	vec3 min = vec3(0, 0, 0);
	vec3 max = vec3(0, 0, 0);

	for (int i = 0; i < Verticies.size(); i++) {
		if (Verticies[i].x < min.x) {
			min.x = Verticies[i].x;
		}
		if (Verticies[i].y < min.y) {
			min.y = Verticies[i].y;
		}
		if (Verticies[i].z < min.z) {
			min.z = Verticies[i].z;
		}
		if (Verticies[i].x > max.x) {
			max.x = Verticies[i].x;
		}
		if (Verticies[i].y > max.y) {
			max.y = Verticies[i].y;
		}
		if (Verticies[i].z > max.z) {
			max.z = Verticies[i].z;
		}
	}

	S_AABBMin = min;
	S_AABBMax = max;
}
FAABB::FAABB(vec3 Min, vec3 Max) : S_AABBMin(Min), S_AABBMax(Max) {

}
FAABB::~FAABB() {}

vec3 FAABB::GetMinExtent() {
	return S_AABBMin;
}
vec3 FAABB::GetMaxExtent() {
	return S_AABBMax;
}
bool FAABB::IntersectWithRay(vec3 RayOrigin, vec3 RayDirection, Transform OwnerTransform, vec3& HitLocation, float& IntersectionDistance) {
	mat4 ModelMatrix = OwnerTransform.GetModelMatrix();
	vec4 aabbMin = ModelMatrix * vec4(S_AABBMin, 1.0f);
	vec4 aabbMax = ModelMatrix * vec4(S_AABBMax, 1.0f);

	return MathLibrary::LineTraceAgainstAABB(RayOrigin, RayDirection, aabbMin, aabbMax, HitLocation, IntersectionDistance);
}
void FAABB::DrawDebug(vec3 Color, Camera* Camera, Transform OwnerTransform) {
	EngineStatics::GetLineShader()->Bind();
	EngineStatics::GetLineShader()->SetShaderVector3("COLOR", Color);
	EngineStatics::GetLineShader()->Update(OwnerTransform, Camera);
	glBegin(GL_LINES);

	//Bottom
	glVertex3f(S_AABBMin.x, S_AABBMin.y, S_AABBMin.z);
	glVertex3f(S_AABBMin.x, S_AABBMax.y, S_AABBMin.z);

	glVertex3f(S_AABBMin.x, S_AABBMin.y, S_AABBMin.z);
	glVertex3f(S_AABBMax.x, S_AABBMin.y, S_AABBMin.z);

	glVertex3f(S_AABBMax.x, S_AABBMax.y, S_AABBMin.z);
	glVertex3f(S_AABBMax.x, S_AABBMin.y, S_AABBMin.z);

	glVertex3f(S_AABBMax.x, S_AABBMax.y, S_AABBMin.z);
	glVertex3f(S_AABBMin.x, S_AABBMax.y, S_AABBMin.z);

	//Top
	glVertex3f(S_AABBMin.x, S_AABBMin.y, S_AABBMax.z);
	glVertex3f(S_AABBMin.x, S_AABBMax.y, S_AABBMax.z);

	glVertex3f(S_AABBMin.x, S_AABBMin.y, S_AABBMax.z);
	glVertex3f(S_AABBMax.x, S_AABBMin.y, S_AABBMax.z);

	glVertex3f(S_AABBMax.x, S_AABBMax.y, S_AABBMax.z);
	glVertex3f(S_AABBMax.x, S_AABBMin.y, S_AABBMax.z);

	glVertex3f(S_AABBMax.x, S_AABBMax.y, S_AABBMax.z);
	glVertex3f(S_AABBMin.x, S_AABBMax.y, S_AABBMax.z);

	//Sides
	glVertex3f(S_AABBMin.x, S_AABBMin.y, S_AABBMin.z);
	glVertex3f(S_AABBMin.x, S_AABBMin.y, S_AABBMax.z);

	glVertex3f(S_AABBMax.x, S_AABBMin.y, S_AABBMin.z);
	glVertex3f(S_AABBMax.x, S_AABBMin.y, S_AABBMax.z);

	glVertex3f(S_AABBMax.x, S_AABBMax.y, S_AABBMin.z);
	glVertex3f(S_AABBMax.x, S_AABBMax.y, S_AABBMax.z);

	glVertex3f(S_AABBMin.x, S_AABBMax.y, S_AABBMin.z);
	glVertex3f(S_AABBMin.x, S_AABBMax.y, S_AABBMax.z);

	glEnd();
}