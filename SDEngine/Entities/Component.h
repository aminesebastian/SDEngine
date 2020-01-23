#pragma once
#include "Entities/Entity.h"
#include "Engine/EngineObject.h"



class Actor;
enum EDrawType;

class Component : public Entity {
public:
	Component(const TString& Name);
	~Component();

	////////
	//GAME//
	////////
	virtual void ComponentBeginPlay();
	virtual void ComponentEndPlay();
	virtual void ComponentTick(float DeltaTime);
	virtual void ComponentDestroy();

	virtual void OnRegisteredToActor(Actor* Owner);
	virtual void OnDeregisteredFromActor();

	Actor* GetOwner();

private:
	Actor* Owner;
};

