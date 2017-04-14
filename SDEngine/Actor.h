#pragma once
#include "Entity.h"
#include "Transform.h"

class Actor : public Entity {
public:
	Actor(const Transform SpawnTransform);
	~Actor();

};

