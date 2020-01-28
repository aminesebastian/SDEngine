#pragma once
#include "Entities/Actors/StaticMeshActor.h"
#include "Core/Assets/Asset.h"

class HeadActor : public StaticMeshActor {
public:
	HeadActor(const TString& Name);
	~HeadActor();
};

