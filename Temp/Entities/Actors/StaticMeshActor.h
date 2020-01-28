#pragma once
#include "Entities/Actor.h"

class StaticMesh;

class StaticMeshActor : public Actor {
public:
	StaticMeshActor(const TString& Name);
	~StaticMeshActor();

	void SetStaticMesh(StaticMesh* Mesh);
	StaticMesh* GetMesh();
};

