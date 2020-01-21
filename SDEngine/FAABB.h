#pragma once
#include <GLM/glm.hpp>
#include <GLEW/glew.h>
#include "TypeDefenitions.h"

class Camera;
class Transform;

using namespace glm;

class FAABB {

public:
	FAABB(SArray<vec3> Verticies);
	FAABB(vec3 Min, vec3 Max);
	~FAABB();

	vec3 GetMinExtent();
	vec3 GetMaxExtent();

	bool IntersectWithRay(vec3 RayOrigin, vec3 RayDirection, Transform OwnerTransform, vec3& HitLocation, float& IntersectionDistance);

	void DrawDebug(vec3 Color, Camera* Camera, Transform OwnerTransform);
private:
	vec3 S_AABBMin;
	vec3 S_AABBMax;
};