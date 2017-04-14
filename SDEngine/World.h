#pragma once
#include <iostream>
#include <vector>

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

	vector<Entity*> GetWorldEntities() { return S_EntityList; }
	vector<Light*> GetWorldLights() { return S_LightList; }

private:
	vector<Entity*> S_EntityList;
	vector<Light*> S_LightList;

	float S_DeltaTime;
	float S_WorldTime;
};

