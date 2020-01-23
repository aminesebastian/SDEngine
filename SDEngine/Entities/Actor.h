#pragma once
#include "Entities/Entity.h"
#include "Utilities/Transform.h"
#include "Utilities/DataStructures/Tree.h"
#include "Utilities/EngineFunctionLibrary.h"
#include "Rendering/RenderTypeDefenitions.h"
#include "UserInterface/DetailsPanelProvider.h"

class Component;
class BaseWidget;

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

	//////////
	//RENDER//
	//////////
	virtual void PreFrameRendered() override;
	virtual void PostFrameRendered() override;
	virtual void DrawAdvanced(Camera* RenderCamera, EDrawType DrawType) override;

	//////////////
	//COMPONENTS//
	//////////////
	void RegisterComponent(Component* Component);
	bool DeregisterComponent(Component* Component);
	SSet<Component*> GetComponents();

	//////////////////
	//USER INTERFACE//
	//////////////////
	bool PopulateDetailsPanel();
	TString GetDetailsPanelName();

protected:
	Component* RootComponent;
	SArray<BaseWidget*> DetailsPanelWidgets;
private:
	SSet<Component*> Components;
};

