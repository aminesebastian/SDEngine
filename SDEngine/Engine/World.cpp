#include "World.h"
#include "Entities/Camera.h"
#include "Entities/Light.h"
#include "Rendering/Shader.h"
#include "Utilities/Math/MathLibrary.h"
#include "Utilities/EngineFunctionLibrary.h"
#include <limits>


UWorld::UWorld() {}
UWorld::~UWorld() {}

SArray<Entity*> UWorld::GetWorldEntities() { return S_EntityList; }
SArray<Light*> UWorld::GetWorldLights() { return S_LightList; }
void UWorld::TickWorld(float DeltaTime) {
	for (int i = 0; i < S_EntityList.size(); i++) {
		if (S_EntityList[i]->NeedsTick()) {
			S_EntityList[i]->Tick(DeltaTime);
		}
	}
}

void UWorld::RegisterEntity(Entity* entity) {
	S_EntityList.push_back(entity);
	if (IsA<Light>(entity)) {
		S_LightList.push_back(Cast<Light>(entity));
	}
}
bool UWorld::DestroyEntity(Entity* entity) {
	for (int i = 0; i < S_EntityList.size(); i++) {
		if (S_EntityList[i] == entity) {
			S_EntityList.erase(S_EntityList.begin() + i);
			if (IsA<Light>(entity)) {
				for (int j = 0; j < S_LightList.size(); j++) {
					if (S_LightList[j] == entity) {
						S_LightList.erase(S_LightList.begin() + j);
					}
				}
			}
			return true;
		}
	}
	return false;
}