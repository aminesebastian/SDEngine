#include "AxisAlignedBoundingBox.h"
#include "Core/DataStructures/DataStructures.h"
#include "Core/Engine/EngineStatics.h"
#include "Core/Objects/CoreTypes/Shader.h"
#include "Core/Objects/Entities/Camera.h"
#include "Core/Utilities/Math/MathLibrary.h"
#include <limits>

AxisAlignedBoundingBox::AxisAlignedBoundingBox(SArray<vec3> Verticies) {
	vec3 min = vec3(0, 0, 0);
	vec3 max = vec3(0, 0, 0);

	for (int i = 0; i < Verticies.Count(); i++) {
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

	MinExtent = min;
	MaxExtent = max;
}
AxisAlignedBoundingBox::AxisAlignedBoundingBox(vec3 Min, vec3 Max) : MinExtent(Min), MaxExtent(Max) {

}
AxisAlignedBoundingBox::~AxisAlignedBoundingBox() {}

vec3 AxisAlignedBoundingBox::GetMinExtent() {
	return MinExtent;
}
vec3 AxisAlignedBoundingBox::GetMaxExtent() {
	return MaxExtent;
}
bool AxisAlignedBoundingBox::IntersectWithRay(vec3 RayOrigin, vec3 RayDirection, FTransform OwnerTransform, vec3& HitLocation, float& IntersectionDistance) {
	mat4 ModelMatrix = OwnerTransform.GetModelMatrix();
	vec4 aabbMin = ModelMatrix * vec4(MinExtent, 1.0f);
	vec4 aabbMax = ModelMatrix * vec4(MaxExtent, 1.0f);

	return MathLibrary::LineTraceAgainstAABB(RayOrigin, RayDirection, aabbMin, aabbMax, HitLocation, IntersectionDistance);
}
void AxisAlignedBoundingBox::DrawDebug(vec3 Color, Camera* Camera, FTransform OwnerTransform) {
	glLineWidth(2);
	EngineStatics::GetLineShader()->Bind();
	EngineStatics::GetLineShader()->SetShaderVector3("COLOR", Color);
	EngineStatics::GetLineShader()->Update(OwnerTransform, Camera);
	glBegin(GL_LINES);

	//Bottom
	glVertex3f(MinExtent.x, MinExtent.y, MinExtent.z);
	glVertex3f(MinExtent.x, MaxExtent.y, MinExtent.z);

	glVertex3f(MinExtent.x, MinExtent.y, MinExtent.z);
	glVertex3f(MaxExtent.x, MinExtent.y, MinExtent.z);

	glVertex3f(MaxExtent.x, MaxExtent.y, MinExtent.z);
	glVertex3f(MaxExtent.x, MinExtent.y, MinExtent.z);

	glVertex3f(MaxExtent.x, MaxExtent.y, MinExtent.z);
	glVertex3f(MinExtent.x, MaxExtent.y, MinExtent.z);

	//Top
	glVertex3f(MinExtent.x, MinExtent.y, MaxExtent.z);
	glVertex3f(MinExtent.x, MaxExtent.y, MaxExtent.z);

	glVertex3f(MinExtent.x, MinExtent.y, MaxExtent.z);
	glVertex3f(MaxExtent.x, MinExtent.y, MaxExtent.z);

	glVertex3f(MaxExtent.x, MaxExtent.y, MaxExtent.z);
	glVertex3f(MaxExtent.x, MinExtent.y, MaxExtent.z);

	glVertex3f(MaxExtent.x, MaxExtent.y, MaxExtent.z);
	glVertex3f(MinExtent.x, MaxExtent.y, MaxExtent.z);

	//Sides
	glVertex3f(MinExtent.x, MinExtent.y, MinExtent.z);
	glVertex3f(MinExtent.x, MinExtent.y, MaxExtent.z);

	glVertex3f(MaxExtent.x, MinExtent.y, MinExtent.z);
	glVertex3f(MaxExtent.x, MinExtent.y, MaxExtent.z);

	glVertex3f(MaxExtent.x, MaxExtent.y, MinExtent.z);
	glVertex3f(MaxExtent.x, MaxExtent.y, MaxExtent.z);

	glVertex3f(MinExtent.x, MaxExtent.y, MinExtent.z);
	glVertex3f(MinExtent.x, MaxExtent.y, MaxExtent.z);

	glEnd();
}