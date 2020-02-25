#pragma once
#include <GLM\glm.hpp>
#include <GLM\gtx\transform.hpp>
#include "Core/Reflection/Reflection.h"

using namespace glm;

SD_STRUCT()
class Transform {
	SD_STRUCT_BODY();
public:
	/*
	@param Position
	+Y -> Right
	-Y -> Left
	+X -> Towards _Camera
	-X -> Away From _Camera
	+Z -> Up
	-Z -> Down
	*/
	Transform(const Vector3D Position = Vector3D(0.0, 0.0, 0.0), const Vector3D Rotation = Vector3D(0.0, 0.0, 0.0), const Vector3D Scale = Vector3D(1, 1, 1)) :
		Location(Position),
		Rotation(Rotation),
		Scale(Scale) {}
	virtual ~Transform();

	const Vector3D& GetLocation() const {
		return Location;
	}
	const Vector3D& GetRotation() const {
		return Rotation;
	}
	const Vector3D& GetScale() const {
		return Scale;
	}

	void SetUniformScale(float ScaleIn);
	void SetLocation(Vector3D LocationIn);
	void SetRotation(Vector3D RotationIn);
	void SetScale(Vector3D ScaleIn);

	void AddLocation(Vector3D LocationIn);
	void AddRotation(Vector3D RotationIn);
	void AddScale(Vector3D ScaleIn);

	inline Matrix4 GetModelMatrix() const {
		Matrix4 posMatrix = translate(Location);
		Matrix4 rotXMatrix = rotate(Rotation.x, Vector3D(1, 0, 0));
		Matrix4 rotYMatrix = rotate(Rotation.y, Vector3D(0, 1, 0));
		Matrix4 rotZMatrix = rotate(Rotation.z, Vector3D(0, 0, 1));
		Matrix4 scaleMatrix = scale(Scale);

		Matrix4 combinedRotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;

		return posMatrix * combinedRotMatrix * scaleMatrix;
	}
	inline Vector3D GetProjectedPosition() {
		Matrix4 posMatrix = translate(Location);
		return posMatrix * vec4(Location, 1.0);
	}
	inline Vector3D GetForwardVector() const {
		Matrix4 rotXMatrix = rotate(Rotation.x, Vector3D(1, 0, 0));
		Matrix4 rotYMatrix = rotate(Rotation.y, Vector3D(0, 1, 0));
		Matrix4 rotZMatrix = rotate(Rotation.z, Vector3D(0, 0, 1));
		Matrix4 combinedRotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;

		return  combinedRotMatrix[0];
	}
	inline Vector3D GetUpVector() const {
		Matrix4 rotXMatrix = rotate(Rotation.x, Vector3D(1, 0, 0));
		Matrix4 rotYMatrix = rotate(Rotation.y, Vector3D(0, 1, 0));
		Matrix4 rotZMatrix = rotate(Rotation.z, Vector3D(0, 0, 1));
		Matrix4 combinedRotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;

		return  combinedRotMatrix[2];
	}
	inline Vector3D GetRightVector() const {
		Matrix4 rotXMatrix = rotate(Rotation.x, Vector3D(1, 0, 0));
		Matrix4 rotYMatrix = rotate(Rotation.y, Vector3D(0, 1, 0));
		Matrix4 rotZMatrix = rotate(Rotation.z, Vector3D(0, 0, 1));
		Matrix4 combinedRotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;

		return  combinedRotMatrix[1];
	}
	inline void SetRotation(float Pitch, float Yaw, float Roll) {
		Rotation.x = Pitch;
		Rotation.y = Yaw;
		Rotation.z = Roll;
	}
	inline Transform operator+(Transform& OtherTransform) {
		Transform newTrans;
		newTrans.Location = GetLocation() + OtherTransform.GetLocation();
		newTrans.Rotation = GetRotation() + OtherTransform.GetRotation();
		newTrans.Scale = GetScale() * OtherTransform.GetScale();
		return newTrans;
	}
	inline Transform operator+(const Transform& OtherTransform) {
		Transform newTrans;
		newTrans.Location = GetLocation() + OtherTransform.GetLocation();
		newTrans.Rotation = GetRotation() + OtherTransform.GetRotation();
		newTrans.Scale = GetScale() * OtherTransform.GetScale();
		return newTrans;
	}
	inline Transform operator+(Transform& OtherTransform) const {
		Transform newTrans;
		newTrans.Location = GetLocation() + OtherTransform.GetLocation();
		newTrans.Rotation = GetRotation() + OtherTransform.GetRotation();
		newTrans.Scale = GetScale() * OtherTransform.GetScale();
		return newTrans;
	}
	inline Transform operator+(const Transform& OtherTransform) const {
		Transform newTrans;
		newTrans.Location = GetLocation() + OtherTransform.GetLocation();
		newTrans.Rotation = GetRotation() + OtherTransform.GetRotation();
		newTrans.Scale = GetScale() * OtherTransform.GetScale();
		return newTrans;
	}
private:
	SD_PROPERTY();
	Vector3D Location;
	SD_PROPERTY();
	Vector3D Rotation;
	SD_PROPERTY();
	Vector3D Scale;
};

