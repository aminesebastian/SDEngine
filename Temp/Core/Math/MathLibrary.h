#pragma once
#include <sstream>
#include <algorithm>
#include <iterator>
#include "Utilities/TypeDefenitions.h"
#include "Physics/CollisionChannels.h"
#include "Core/DataStructures/DataStructures.h"

using namespace glm;

#define ERROR_EPSILON 1e-8

class Entity;
class World;
class Camera;
template<typename T>
class SArray;

struct FHitInfo {
	bool bHit;
	Entity* HitEntity;
	float HitDistance;
	vec3 HitLocation;

	FHitInfo() {
		bHit = false;
		HitEntity = nullptr;
		HitDistance = -1.0f;
		HitLocation = vec3(0.0f, 0.0f, 0.0f);
	}
};

class MathLibrary {
public:
	MathLibrary();
	~MathLibrary();

	template<typename T>
	static T RandRange(T Min, T Max) {
		return Min + static_cast <T> (rand()) / (static_cast <T> (RAND_MAX / (Max - Min)));
	}
	template<typename T>
	static T Lerp(T Min, T Max, float Alpha) {
		return Min + Alpha * (Max - Min);
	}

	static void ScreenCoordinatesToWorldRay(vec2 MouseCoordinates, vec2 ScreenDimensions, mat4 ViewMatrix, mat4 ProjectionMatrix, vec3& RayOriginOut, vec3& RayDirectionOut);
	static bool LineTraceAgainstWorldFromScreen(FHitInfo& HitInfo, vec2 MouseCoordinates, Camera* Camera, World* World, ECollisionChannel Channel = VISIBILITY);
	static bool LineTraceAgainstWorld(FHitInfo& HitInfo, World* World, vec3 RayOrigin, vec3 RayDirection, ECollisionChannel Channel = VISIBILITY);
	/**
	 * Intersects a ray against a triangle.
	 *
	 * @param RayOrigin The origin of the Ray.
	 * @param RayDirection The direction of the Ray.
	 * @param A One vertex of the triangle.
	 * @param B Second vertex of the triangle.
	 * @param C Third vertex of the triangle.
	 *
	 * @return True if the ray intersects the triangle, false if it does not.
	 */
	static bool LineTraceAgainstTriangle(vec3 RayOrigin, vec3 RayDirection, vec3 A, vec3 B, vec3 C, float& HitDistance);
	static bool LineTraceAgainstTriangles(vec3 RayOrigin, vec3 RayDirection, SArray<vec3> Verticies, SArray<uint32> Indecies, vec3& HitLocation, float& Distance);
	static bool LineTraceAgainstAABB(vec3 RayOrigin, vec3 RayDirection, vec3 AABBMin, vec3 AABBMax, vec3& HitLocation, float& Distance);

	static inline TString LocationToString(vec3 Position) { return "X: " + std::to_string(Position.x) + " Y: " + std::to_string(Position.y) + " Z: " + std::to_string(Position.z); }
	static inline TString RotationToString(vec3 EulerRotation) { return "P: " + std::to_string(degrees(EulerRotation.x)) + " Y: " + std::to_string(degrees(EulerRotation.y)) + " R: " + std::to_string(degrees(EulerRotation.z)); }
	static inline TString ScaleToString(vec3 Scale) { return "X: " + std::to_string(Scale.x) + " Y: " + std::to_string(Scale.y) + " Z: " + std::to_string(Scale.z); }

	static inline TString SerializeVector(vec2 Vector) { return std::to_string(Vector.x) + " " + std::to_string(Vector.y); }
	static inline TString SerializeVector(vec3 Vector) { return std::to_string(Vector.x) + " " + std::to_string(Vector.y) + " " + std::to_string(Vector.z); }
	static inline TString SerializeVector(vec4 Vector) { return std::to_string(Vector.x) + " " + std::to_string(Vector.y) + " " + std::to_string(Vector.z) + " " + std::to_string(Vector.w); }

	static inline vec2 DeserializeVector2(TString SerializedVector) {
		vec2 output;
		std::stringstream ss(SerializedVector.ToCString());
		std::string token;
		int curr = 0;
		while (std::getline(ss, token, ' ')) {
			output[curr] = std::stof(token);
		}
		return output;
	}
	static inline vec3 DeserializeVector3(TString SerializedVector) {
		vec3 output;
		std::stringstream ss(SerializedVector.ToCString());
		std::string token;
		int curr = 0;
		while (std::getline(ss, token, ' ')) {
			output[curr] = std::stof(token);
		}
		return output;
	}
	static inline vec4 DeserializeVector4(TString SerializedVector) {
		vec4 output;
		std::stringstream ss(SerializedVector.ToCString());
		std::string token;
		int curr = 0;
		while (std::getline(ss, token, ' ')) {
			output[curr] = std::stof(token);
		}
		return output;
	}
};

