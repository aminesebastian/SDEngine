#pragma once
#include <iostream>
#include <vector>
#include "Entity.h"

class World {
public:
	World();
	virtual ~World();

	void DrawWorld();

private:
	std::vector<Entity*> S_EntityList;
};

