#pragma once
#include "Transform.h"
#include "Shader.h"

class Entity {

public:
	Entity(const Transform& SpawnTransform) :
		S_Transform(SpawnTransform) {}

	Entity() {}
	virtual ~Entity();

	virtual void Draw(Shader& shader) {}

	Transform& GetTransform() { return S_Transform; }
protected:
	Transform S_Transform;
};

