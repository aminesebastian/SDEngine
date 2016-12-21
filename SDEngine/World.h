#pragma once
#include <iostream>
#include <vector>
#include <GLM\glm.hpp>
#include <GLM\gtx\transform.hpp>


class Entity;
class Light;

class World {
public:
	World();
	virtual ~World();

	void DrawWorld();
	void GetWorldBounds(glm::vec3& MinBound, glm::vec3& MaxBound);
	void Tick(float DeltaTime);
private:
	std::vector<Entity*> S_EntityList;
	std::vector<Light*> S_LightMapLights;
};

