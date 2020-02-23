#pragma once
#include "Core/Objects/Entities/Actor.h"
#include "Core/Objects/CoreTypes/StaticMesh.h"
#include "Core/Objects/Entities/Components/StaticMeshComponent.h"

class StaticMeshActor : public Actor {
public:
	StaticMeshActor(const TString& Name);
	~StaticMeshActor();

	void SetStaticMesh(StaticMesh* Mesh);
	StaticMesh* GetMesh();
};

