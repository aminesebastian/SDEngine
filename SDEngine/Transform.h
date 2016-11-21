#pragma once
#include <GLM\glm.hpp>
#include <GLM\gtx\transform.hpp>

using namespace glm;

class Transform {

public:
	Transform(const vec3& Position = vec3(), const vec3& Rotation = vec3(), const vec3& Scale = vec3(1,1,1)) :
		S_Position(Position),
		S_Rotation(Rotation),
		S_Scale(Scale) {}

	virtual ~Transform();

	vec3& GetPosition() { return S_Position; }
	vec3& GetRotation() { return S_Rotation; }
	vec3& GetScale() { return S_Scale; }

	inline mat4 GetModelMatrix() const {
		mat4 posMatrix = translate(S_Position);
		mat4 rotXMatrix = rotate(S_Rotation.x, vec3(1, 0, 0));
		mat4 rotYMatrix = rotate(S_Rotation.y, vec3(0, 1, 0));
		mat4 rotZMatrix = rotate(S_Rotation.z, vec3(0, 0, 1));
		mat4 scaleMatrix = scale (S_Scale);

		mat4 combinedRotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;

		return posMatrix * combinedRotMatrix * scaleMatrix;
	}

private:
	vec3 S_Position;
	vec3 S_Rotation;
	vec3 S_Scale;
};

