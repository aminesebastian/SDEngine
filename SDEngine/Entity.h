#pragma once
#include "Transform.h"

class Entity {

public:
	Entity(const Transform& SpawnTransform) :
		S_Transform(SpawnTransform) {}

	Entity() {}
	virtual ~Entity();

	virtual void Draw() {}

	Transform& GetTransform() { return S_Transform; }
protected:
	Transform S_Transform;
};

