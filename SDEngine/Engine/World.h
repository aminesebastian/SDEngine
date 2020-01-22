#pragma once
#include <iostream>
#include "Utilities/TypeDefenitions.h"

class Entity;
class Camera;
class Shader;
class Light;

using namespace std;

class UWorld {
public:
	UWorld();
	virtual ~UWorld();

	void TickWorld(float DeltaTime);

	void RegisterEntity(Entity* entity);
	bool DestroyEntity(Entity* entity);

	SArray<Entity*> GetWorldEntities();
	SArray<Light*> GetWorldLights();

private:
	SArray<Entity*> S_EntityList;
	SArray<Light*>  S_LightList;
};

