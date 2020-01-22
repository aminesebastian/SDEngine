#pragma once
#include "Utilities/TypeDefenitions.h"

class Engine;
class WorkerThread;

class Scene {
public:
	Scene();
	~Scene();

	bool LoadScene();
	bool SaveScene();

private:
	void LoadPlane();
	void LoadHead();
	void LoadSponza();
	void LoadLights();

	vector<WorkerThread*> m_ActiveThreads;
};

