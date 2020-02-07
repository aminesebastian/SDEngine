#include "World.h"
#include "Core/Objects/Entities/Camera.h"
#include "Core/Objects/Entities/Light.h"
#include "Core/Objects/Entities/Actor.h"
#include "Core/Objects/CoreTypes/Shader.h"
#include "Core/Utilities/Math/MathLibrary.h"
#include "Core/Utilities/EngineFunctionLibrary.h"
#include <limits>


World::World() {}
World::~World() {}

SArray<Actor*> World::GetWorldActors() { return ActorList; }
SArray<Light*> World::GetWorldLights() { return LightList; }
void World::TickWorld(float DeltaTime) {
	for (int i = 0; i < ActorList.Count(); i++) {
		ActorList[i]->Tick(DeltaTime);
	}
}

void World::RegisterActor(Actor* ActorToRegister) {
	ActorList.Add(ActorToRegister);
	if (IsA<Light>(ActorToRegister)) {
		LightList.Add(Cast<Light>(ActorToRegister));
	}
}
bool World::DestroyActor(Actor* ActorToDestroy) {
	if (ActorList.Remove(ActorToDestroy)) {
		if (IsA<Light>(ActorToDestroy)) {
			LightList.Remove(Cast<Light>(ActorToDestroy));
		}
		return true;
	}
	return false;
}