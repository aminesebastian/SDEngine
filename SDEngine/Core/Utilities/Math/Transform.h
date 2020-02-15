#pragma once
#include <GLM\glm.hpp>
#include <GLM\gtx\transform.hpp>
#include <string>

using namespace glm;

class Transform {

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
	Transform(const vec3 Position = vec3(0.0, 0.0, 0.0), const vec3 Rotation = vec3(0.0, 0.0, 0.0), const vec3 Scale = vec3(1, 1, 1)) :
		Location(Position),
		Rotation(Rotation),
		Scale(Scale) {}
	virtual ~Transform();

	const vec3& GetLocation() const {
		return Location;
	}
	const vec3& GetRotation() const {
		return Rotation;
	}
	const vec3& GetScale() const {
		return Scale;
	}

	void SetUniformScale(float ScaleIn);
	void SetLocation(vec3 LocationIn);
	void SetRotation(vec3 RotationIn);
	void SetScale(vec3 ScaleIn);

	void AddLocation(vec3 LocationIn);
	void AddRotation(vec3 RotationIn);
	void AddScale(vec3 ScaleIn);

	inline mat4 GetModelMatrix() const {
		mat4 posMatrix = translate(Location);
		mat4 rotXMatrix = rotate(Rotation.x, vec3(1, 0, 0));
		mat4 rotYMatrix = rotate(Rotation.y, vec3(0, 1, 0));
		mat4 rotZMatrix = rotate(Rotation.z, vec3(0, 0, 1));
		mat4 scaleMatrix = scale(Scale);

		mat4 combinedRotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;

		return posMatrix * combinedRotMatrix * scaleMatrix;
	}
	inline vec3 GetProjectedPosition() {
		mat4 posMatrix = translate(Location);
		return posMatrix * vec4(Location, 1.0);
	}
	inline vec3 GetForwardVector() const {
		mat4 rotXMatrix = rotate(Rotation.x, vec3(1, 0, 0));
		mat4 rotYMatrix = rotate(Rotation.y, vec3(0, 1, 0));
		mat4 rotZMatrix = rotate(Rotation.z, vec3(0, 0, 1));
		mat4 combinedRotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;

		return  combinedRotMatrix[0];
	}
	inline vec3 GetUpVector() const {
		mat4 rotXMatrix = rotate(Rotation.x, vec3(1, 0, 0));
		mat4 rotYMatrix = rotate(Rotation.y, vec3(0, 1, 0));
		mat4 rotZMatrix = rotate(Rotation.z, vec3(0, 0, 1));
		mat4 combinedRotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;

		return  combinedRotMatrix[2];
	}
	inline vec3 GetRightVector() const {
		mat4 rotXMatrix = rotate(Rotation.x, vec3(1, 0, 0));
		mat4 rotYMatrix = rotate(Rotation.y, vec3(0, 1, 0));
		mat4 rotZMatrix = rotate(Rotation.z, vec3(0, 0, 1));
		mat4 combinedRotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;

		return  combinedRotMatrix[1];
	}
	inline void SetRotation(float Pitch, float Yaw, float Roll) {
		Rotation.x = Pitch;
		Rotation.y = Yaw;
		Rotation.z = Roll;
	}
	inline std::string LocationToString() { return "X: " + std::to_string(Location.x) + "Y: " + std::to_string(Location.y) + "Z: " + std::to_string(Location.z); }
	inline std::string RotationToString() { return "P: " + std::to_string(degrees(Rotation.x)) + "Y: " + std::to_string(degrees(Rotation.y)) + "R: " + std::to_string(degrees(Rotation.z)); }
	inline std::string ScaleToString() { return "X: " + std::to_string(Scale.x) + "Y: " + std::to_string(Scale.y) + "Z: " + std::to_string(Scale.z); }
	inline std::string ForwardVectorToString() {
		vec3 tempForward = GetForwardVector();
		return "X: " + std::to_string(tempForward.x) + "Y: " + std::to_string(tempForward.y) + "Z: " + std::to_string(tempForward.z);
	}
	inline std::string UpVectorToString() {
		vec3 tempForward = GetUpVector();
		return "X: " + std::to_string(tempForward.x) + "Y: " + std::to_string(tempForward.y) + "Z: " + std::to_string(tempForward.z);
	}
	inline std::string RightVectorToString() {
		vec3 tempForward = GetRightVector();
		return "X: " + std::to_string(tempForward.x) + "Y: " + std::to_string(tempForward.y) + "Z: " + std::to_string(tempForward.z);
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
	vec3 Location;
	vec3 Rotation;
	vec3 Scale;
};

