#pragma once
#include <iostream>
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/DataStructures/DataStructures.h"

class Actor;
class Camera;
class Shader;
class Light;

using namespace std;

class World {
public:
	World();
	virtual ~World();

	void TickWorld(float DeltaTime);

	void RegisterActor(Actor* ActorToRegister);
	bool DestroyActor(Actor* ActorToDestroy);

	SArray<Actor*> GetWorldActors();
	SArray<Light*> GetWorldLights();

private:
	SArray<Actor*> ActorList;
	SArray<Light*> LightList;
};

