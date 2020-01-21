#pragma once
#include <iostream>
#include "TypeDefenitions.h"

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

	void RegisterLight(Light* light);
	bool DestroyLight(Light* light);

	SArray<Entity*> GetWorldEntities();
	SArray<Light*> GetWorldLights();

private:
	SArray<Entity*> S_EntityList;
	SArray<Light*>  S_LightList;

	float S_DeltaTime;
	float S_WorldTime;
};

