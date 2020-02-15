#pragma once
#include "Core/Objects/Entities/Entity.h"
#include "Core/Utilities/Math/Transform.h"
#include "Core/DataStructures/DataStructures.h"
#include "Core/Utilities/EngineFunctionLibrary.h"
#include "Core/Rendering/RenderTypeDefenitions.h"
#include "UserInterface/DetailsPanelProvider.h"

class Component;
class DragFloat;

class Actor : public Entity, public IDetailsPanelProvider {
public:
	Actor(const TString& Name);
	~Actor();

	////////
	//GAME//
	////////
	virtual void BeginPlay();
	virtual void EndPlay();
	virtual void Tick(float DeltaTime);
	virtual void Destroy();

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
	void RegisterComponent(Component* Component);
	bool DeregisterComponent(Component* Component);
	SSet<Component*> GetComponents();

	//////////
	//RENDER//
	//////////
	virtual void PreFrameRendered() override;
	virtual void PostFrameRendered() override;
	virtual void DrawAdvanced(const Camera* RenderCamera, const EDrawType& DrawType) override;

	//////////////////
	//USER INTERFACE//
	//////////////////
	virtual bool PopulateDetailsPanel() override;
	virtual TString GetDetailsPanelName() override;

protected:
	Component* RootComponent;
	SArray<DragFloat*> DetailsPanelWidgets;

private:
	SSet<Component*> Children;
};

