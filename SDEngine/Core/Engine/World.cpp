#include "World.h"
#include "Core/Objects/Entities/Camera.h"
#include "Core/Objects/Entities/Light.h"
#include "Core/Objects/Entities/Actor.h"
#include "Core/Objects/CoreTypes/Shader.h"
#include "Core/Utilities/Math/MathLibrary.h"
#include "Core/Utilities/EngineFunctionLibrary.h"
#include <limits>


World::World() {
	WorldTime      = 0.0f;
	LastFrameTime  = 0.0f;
	bGameMode      = true;
	SelectedEntity = nullptr;
}
World::~World() {}

void World::TickWorld(const float& DeltaTime) {
	for (int i = 0; i < ActorList.Count(); i++) {
		ActorList[i]->Tick(DeltaTime);
	}
}
void World::UpdateFrameTime(const float& DeltaTime) {
	WorldTime += DeltaTime;
	LastFrameTime = DeltaTime;
}
void World::RegisterActor(Actor* ActorToRegister) {
	ActorList.Add(ActorToRegister);
	if (IsA<Light>(ActorToRegister)) {
		LightList.Add(Cast<Light>(ActorToRegister));
	}
	ActorToRegister->OnAddedToWorld(this);
}
bool World::DestroyActor(Actor* ActorToDestroy) {
	if (ActorList.Remove(ActorToDestroy)) {
		if (IsA<Light>(ActorToDestroy)) {
			LightList.Remove(Cast<Light>(ActorToDestroy));
		}
		ActorToDestroy->OnRemovedFromWorld();
		return true;
	}
	return false;
}

SArray<Actor*> World::GetWorldActors() const {
	return ActorList;
}
SArray<Light*> World::GetWorldLights() const {
	return LightList;
}
const float& World::GetWorldTime() const {
	return WorldTime;
}
const float& World::GetLastFrameTime() const {
	return LastFrameTime;
}
const bool& World::IsInGameMode() const {
	return bGameMode;
}
void World::SetGameMode(const bool& GameMode) {
	bGameMode = GameMode;
}
Entity* World::GetSelectedEntity() const {
	return SelectedEntity;
}
void World::SetSelectedEntity(Entity* SelectedEntityIn) {
	SelectedEntity = SelectedEntityIn;
}