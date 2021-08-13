#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Physics/CollisionChannels.h"
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
	static T RandRange(const T& Minimum, const T& Maximum) {
		return Minimum + static_cast <T> (rand()) / (static_cast <T> (RAND_MAX / (Maximum - Minimum)));
	}
	template<typename T>
	static T Lerp(const T& Minimum, const T& Maximum, float Alpha) {
		return Minimum + Alpha * (Maximum - Minimum);
	}
	template<typename T>
	static T Max(const T& Value, const T& Maximum) {
		return Value > Maximum ? Value : Maximum;
	}
	template<typename T>
	static T Min(const T& Value, const T& Minimum) {
		return Value < Minimum ? Value : Minimum;
	}
	template<typename T>
	static T Clamp(const T& Value, const T& Minimum, const T& Maximum) {
		return Min(Max(Value, Minimum), Maximum);
	}

	static Vector2D MinVector(const Vector2D& First, const Vector2D& Second);
	static Vector2D MaxVector(const Vector2D& First, const Vector2D& Second);
	static Vector3D MinVector(const Vector3D& First, const Vector3D& Second);
	static Vector3D MaxVector(const Vector3D& First, const Vector3D& Second);
	static Vector4D MinVector(const Vector4D& First, const Vector4D& Second);
	static Vector4D MaxVector(const Vector4D& First, const Vector4D& Second);

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

	static bool PointIntersect2DBox(const Vector2D& Point, const Vector2D& BoxMinExtent, const Vector2D& BoxMaxExtent);

	static inline TString LocationToString(vec2 Position) { return "X: " + std::to_string(Position.x) + " Y: " + std::to_string(Position.y); }
	static inline TString LocationToString(vec3 Position) { return "X: " + std::to_string(Position.x) + " Y: " + std::to_string(Position.y) + " Z: " + std::to_string(Position.z); }
	static inline TString RotationToString(vec3 EulerRotation) { return "P: " + std::to_string(degrees(EulerRotation.x)) + " Y: " + std::to_string(degrees(EulerRotation.y)) + " R: " + std::to_string(degrees(EulerRotation.z)); }
	static inline TString ScaleToString(vec3 Scale) { return "X: " + std::to_string(Scale.x) + " Y: " + std::to_string(Scale.y) + " Z: " + std::to_string(Scale.z); }

	static Vector2D ConvertAbsoluteToRelativeScreenCoordinates(const Vector2D& Coordinates, const Vector2D& ScreenResolution);
	static Vector2D ConvertAbsoluteToNdcScreenCoordinates(const Vector2D& Coordinates, const Vector2D& ScreenResolution);
	static Vector2D ConvertNdcToRelativeScreenCoordinates(const Vector2D& Coordinates);
	static Vector2D ConvertNdcToAbsoluteScreenCoordinates(const Vector2D& Coordinates, const Vector2D& ScreenResolution);
	static Vector2D ConvertRelativeToNdcScreenCoordinates(const Vector2D& Coordinates);
	static Vector2D ConvertRelativeToAbsoluteScreenCoordinates(const Vector2D& Coordinates, const Vector2D& ScreenResolution);
};

