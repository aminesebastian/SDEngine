#include "World.h"
#include "Camera.h"
#include "Light.h"
#include "Shader.h"
#include "MathLibrary.h"
#include <limits>


UWorld::UWorld() : S_DeltaTime(0), S_WorldTime(0) {}
UWorld::~UWorld() {
	delete &S_EntityList;
	delete &S_LightList;
}

SArray<Entity*> UWorld::GetWorldEntities() { return S_EntityList; }
SArray<Light*> UWorld::GetWorldLights() { return S_LightList; }
void UWorld::TickWorld(float DeltaTime) {
	for (int i = 0; i < S_EntityList.size(); i++) {
		if (S_EntityList[i]->NeedsTick()) {
			S_EntityList[i]->Tick(DeltaTime);
		}
	}
	for (int i = 0; i < S_LightList.size(); i++) {
		if (S_LightList[i]->NeedsTick()) {
			S_LightList[i]->Tick(DeltaTime);
		}
	}
}

void UWorld::RegisterEntity(Entity* entity) {
	S_EntityList.push_back(entity);
}
bool UWorld::DestroyEntity(Entity* entity) {
	for (int i = 0; i < S_EntityList.size(); i++) {
		if (S_EntityList[i] == entity) {
			S_EntityList.erase(S_EntityList.begin() + i);
			return true;
		}
	}
	return false;
}

void UWorld::RegisterLight(Light* light) {
	S_LightList.push_back(light);
}
bool UWorld::DestroyLight(Light* light) {
	for (int i = 0; i < S_LightList.size(); i++) {
		if (S_LightList[i] == light) {
			S_LightList.erase(S_LightList.begin() + i);
			return true;
		}
	}
	return false;
}
