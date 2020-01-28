#pragma once
#include <GLM/glm.hpp>
#include <GLEW/glew.h>
#include "Core/DataStructures/DataStructures.h"
#include "Core/DataTypes/TypeDefenitions.h"
#include "Utilities/Transform.h"

class Camera;
class CurrentTransform;

using namespace glm;

class AxisAlignedBoundingBox {

public:
	AxisAlignedBoundingBox(SArray<vec3> Verticies);
	AxisAlignedBoundingBox(vec3 Min, vec3 Max);
	~AxisAlignedBoundingBox();

	vec3 GetMinExtent();
	vec3 GetMaxExtent();

	bool IntersectWithRay(vec3 RayOrigin, vec3 RayDirection, Transform OwnerTransform, vec3& HitLocation, float& IntersectionDistance);

	void DrawDebug(vec3 Color, Camera* Camera, Transform OwnerTransform);
private:
	vec3 MinExtent;
	vec3 MaxExtent;
};