#pragma once
#include <iostream>
#include <vector>

class Entity;

class World {
public:
	World();
	virtual ~World();

	void DrawWorld();

private:
	std::vector<Entity*> S_EntityList;
};

