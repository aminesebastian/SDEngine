#include "World.h"
#include "Entities/Camera.h"
#include "Entities/Light.h"
#include "Entities/Actor.h"
#include "Rendering/Shader.h"
#include "Core/Math/MathLibrary.h"
#include "Utilities/EngineFunctionLibrary.h"
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