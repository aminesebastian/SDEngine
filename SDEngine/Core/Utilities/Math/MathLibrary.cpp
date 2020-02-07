#include "MathLibrary.h"
#include <stdio.h>      
#include <stdlib.h>    
#include <time.h>     
#include <iostream>
#include "Core/Objects/Entities/Entity.h"
#include "Core/Engine/World.h"
#include "Core/Objects/Entities/Light.h"
#include "Core/Objects/Entities/Camera.h"
#include "Core/DataStructures/DataStructures.h"

MathLibrary::MathLibrary() {}
MathLibrary::~MathLibrary() {}

void MathLibrary::ScreenCoordinatesToWorldRay(vec2 MouseCoordinates, vec2 ScreenDimensions, mat4 ViewMatrix, mat4 ProjectionMatrix, vec3& RayOriginOut, vec3& RayDirectionOut) {
	// The ray Start and End positions, in Normalized Device Coordinates (Have you read Tutorial 4 ?)
	MouseCoordinates.y = ScreenDimensions.y - MouseCoordinates.y;
	glm::vec4 lRayStart_NDC(
		((float)MouseCoordinates.x / (float)ScreenDimensions.x - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
		((float)MouseCoordinates.y / (float)ScreenDimensions.y - 0.5f) * 2.0f, // [0, 768] -> [-1,1]
		-1.0f, // The near plane maps to Z=-1 in Normalized Device Coordinates
		1.0f
	);
	glm::vec4 lRayEnd_NDC(
		((float)MouseCoordinates.x / (float)ScreenDimensions.x - 0.5f) * 2.0f,
		((float)MouseCoordinates.y / (float)ScreenDimensions.y - 0.5f) * 2.0f,
		0.0f,
		1.0f
	);

	glm::mat4 M = glm::inverse(ProjectionMatrix * ViewMatrix);

	glm::vec4 lRayStart_world = M * lRayStart_NDC;
	lRayStart_world /= lRayStart_world.w;

	glm::vec4 lRayEnd_world = M * lRayEnd_NDC;
	lRayEnd_world /= lRayEnd_world.w;


	glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
	lRayDir_world = glm::normalize(lRayDir_world);


	RayOriginOut = glm::vec3(lRayStart_world);
	RayDirectionOut = glm::normalize(lRayDir_world);
}

bool MathLibrary::LineTraceAgainstWorldFromScreen(FHitInfo& HitInfo, vec2 MouseCoordinates, Camera* Camera, World* World, ECollisionChannel Channel) {
	vec3 origin;
	vec3 direction;
	ScreenCoordinatesToWorldRay(MouseCoordinates, Camera->GetRenderTargetDimensions(), Camera->GetViewMatrix(), Camera->GetProjectionMatrix(), origin, direction);
	return LineTraceAgainstWorld(HitInfo, World, origin, direction, Channel);
}
bool MathLibrary::LineTraceAgainstWorld(FHitInfo& HitInfo, World* World, vec3 RayOrigin, vec3 RayDirection, ECollisionChannel Channel) {
	// Preallocate pointer to closest hit entity.
	Entity* hitEntity = nullptr;

	// Keep track of the minimum distance. If an intersected entity is closer than this value, it is the newest closest entity.
	float minDistance = std::numeric_limits<float>::max();
	vec3 minCollisionPoint = vec3(0.0f, 0.0f, 0.0f);

	// Check all world entities.
	for (Entity* temp : World->GetWorldActors()) {
		/*if (temp->IsVisible() && temp->HasAxisAlignedBoundingBox()) {
			float dist;
			vec3 collisionPoint;
			bool collide = temp->TraceAgainstRay(RayOrigin, RayDirection, collisionPoint, dist, Channel);

			if (collide) {
				if (dist < minDistance) {
					minDistance = dist;
					hitEntity = temp;
					minCollisionPoint = collisionPoint;
				}
			}
		}*/
	}

	if (hitEntity) {
		HitInfo.bHit = true;
		HitInfo.HitEntity = hitEntity;
		HitInfo.HitDistance = minDistance;
		HitInfo.HitLocation = minCollisionPoint;
	} else {
		HitInfo.bHit = false;
	}

	return HitInfo.bHit;
}
bool MathLibrary::LineTraceAgainstTriangle(vec3 RayOrigin, vec3 RayDirection, vec3 A, vec3 B, vec3 C, float& HitDistance) {
	float uCoord, vCoord = 0.0f;
	vec3 AB    = B - A;
	vec3 AC    = C - A;
	vec3 pvec  = glm::cross(RayDirection, AC);
	float det  = glm::dot(AB, pvec);

	// ray and triangle are parallel if det is close to 0
	if (fabs(det) < ERROR_EPSILON) {
		return false;
	}

	float invDet = 1 / det;

	vec3 tvec = RayOrigin - A;
	uCoord = glm::dot(tvec, pvec) * invDet;
	if (uCoord < 0 || uCoord > 1) {
		return false;
	}

	vec3 qvec = glm::cross(tvec, AB);
	vCoord = glm::dot(RayDirection, qvec) * invDet;
	if (vCoord < 0 || uCoord + vCoord > 1) {
		return false;
	}

	HitDistance = glm::dot(AC, qvec) * invDet;

	return true;
}
bool MathLibrary::LineTraceAgainstTriangles(vec3 RayOrigin, vec3 RayDirection, SArray<vec3> Verticies, SArray<uint32> Indecies, vec3& HitLocation, float& Distance) {
	float distance = 0.0f;
	for (int32 i = 0; i < Indecies.Count() - 2; i += 3) {
		if (LineTraceAgainstTriangle(RayOrigin, RayDirection, Verticies[Indecies[i]], Verticies[Indecies[i + 1]], Verticies[Indecies[i + 2]], distance)) {
			HitLocation = RayOrigin + (RayDirection * Distance);
			Distance = distance;
			return true;
		}
	}
	return false;
}
bool MathLibrary::LineTraceAgainstAABB(vec3 RayOrigin, vec3 RayDirection, vec3 AABBMin, vec3 AABBMax, vec3& HitLocation, float& Distance) {
	float tmin = (AABBMin.x - RayOrigin.x) / RayDirection.x;
	float tmax = (AABBMax.x - RayOrigin.x) / RayDirection.x;
	float tymin = (AABBMin.y - RayOrigin.y) / RayDirection.y;
	float tymax = (AABBMax.y - RayOrigin.y) / RayDirection.y;

	// Ensure that the mins are always less than the maxes.
	if (tmin > tmax) {
		swap(tmin, tmax);
	}
	if (tymin > tymax) {
		swap(tymin, tymax);
	}

	if ((tmin > tymax) || (tymin > tmax)) {
		return false;
	}

	if (tymin > tmin) {
		tmin = tymin;
	}
	if (tymax < tmax) {
		tmax = tymax;
	}
	float tzmin = (AABBMin.z - RayOrigin.z) / RayDirection.z;
	float tzmax = (AABBMax.z - RayOrigin.z) / RayDirection.z;

	if (tzmin > tzmax) {
		swap(tzmin, tzmax);
	}
	if ((tmin > tzmax) || (tzmin > tmax)) {
		return false;
	}
	if (tzmin > tmin) {
		tmin = tzmin;
	}
	if (tzmax < tmax) {
		tmax = tzmax;
	}

	Distance = tmin;
	HitLocation = RayOrigin + (RayDirection * Distance);

	return true;
}