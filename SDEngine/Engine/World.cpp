#include "World.h"
#include "Entities/Camera.h"
#include "Entities/Light.h"
#include "Entities/Actor.h"
#include "Rendering/Shader.h"
#include "Utilities/Math/MathLibrary.h"
#include "Utilities/EngineFunctionLibrary.h"
#include <limits>


World::World() {}
World::~World() {}

SArray<Actor*> World::GetWorldActors() { return ActorList; }
SArray<Light*> World::GetWorldLights() { return LightList; }
void World::TickWorld(float DeltaTime) {
	for (int i = 0; i < ActorList.size(); i++) {
		ActorList[i]->Tick(DeltaTime);
	}
}

void World::RegisterActor(Actor* ActorToRegister) {
	ActorList.push_back(ActorToRegister);
	if (IsA<Light>(ActorToRegister)) {
		LightList.push_back(Cast<Light>(ActorToRegister));
	}
}
bool World::DestroyActor(Actor* ActorToDestroy) {
	for (int i = 0; i < ActorList.size(); i++) {
		if (ActorList[i] == ActorToDestroy) {
			ActorList.erase(ActorList.begin() + i);
			if (IsA<Light>(ActorToDestroy)) {
				for (int j = 0; j < LightList.size(); j++) {
					if (LightList[j] == ActorToDestroy) {
						LightList.erase(LightList.begin() + j);
					}
				}
			}
			return true;
		}
	}
	return false;
}