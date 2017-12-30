#pragma once
#include "Entity.h"
#include "Transform.h"

class Actor : public Entity {
public:
	Actor(TString Name, const Transform SpawnTransform);
	~Actor();

};

