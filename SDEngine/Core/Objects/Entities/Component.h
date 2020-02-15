#pragma once
#include "Core/Objects/Entities/Entity.h"
#include "Core/Objects/EngineObject.h"



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

	virtual void OnAttachedToComponent(Component* Component);
	virtual void OnDetatchedFromComponent();

	Actor* GetOwningActor();

	////////////
	//LOCATION//
	////////////
	vec3 GetWorldLocation();
	vec3 GetWorldRotation();
	vec3 GetWorldScale();
	Transform GetWorldTransform();
	Transform GetLastFrameWorldTransform();

	//////////////
	//COMPONENTS//
	//////////////
	void AddChild(Component* Component);
	bool RemoveChild(Component* Component);
	SSet<Component*> GetChildren(bool bAllDescendants);

	/////////////
	//RENDERING//
	/////////////
	virtual void PostFrameRendered() override;

private:
	Actor* OwningActor;
	Component* OwningComponent;
	SSet<Component*> Children;

	void GetAllChildrenRecursive(Component* Root, SSet<Component*>& Components);
};

