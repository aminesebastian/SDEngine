#pragma once
#include "Entities/Entity.h"
#include "Utilities/Transform.h"

class Actor : public Entity {
public:
	Actor(TString Name, const Transform SpawnTransform);
	~Actor();

};

