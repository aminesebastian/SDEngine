#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/DataStructures/DataStructures.h"

class Actor;
class Camera;
class Shader;
class Light;
class Entity;

using namespace std;

class World {
public:
	World();
	virtual ~World();

	void TickWorld(const float& DeltaTime);
	void RegisterActor(Actor* ActorToRegister);
	bool DestroyActor(Actor* ActorToDestroy);

	void UpdateFrameTime(const float& DeltaTime);

	SArray<Actor*> GetWorldActors() const;
	SArray<Light*> GetWorldLights() const;
	const float& GetWorldTime() const;
	const float& GetLastFrameTime() const;
	const bool& IsInGameMode() const;
	void SetGameMode(const bool& GameMode);

	Entity* GetSelectedEntity() const;
private:
	SArray<Actor*> ActorList;
	SArray<Light*> LightList;
	float WorldTime;
	float LastFrameTime;
	bool bGameMode;
	Entity* SelectedEntity;
};

