#pragma once
#include "Core/DataTypes/TypeDefenitions.h"

class Engine;
class WorkerThread;
class World;

class Scene {
public:
	Scene();
	~Scene();

	bool LoadScene(World* WorldIn);
	bool UnloadScene(World* WorldIn);
	bool SaveScene();

private:
	void LoadPlane(World* WorldIn);
	void LoadHead(World* WorldIn);
	void LoadLights(World* WorldIn);
};

