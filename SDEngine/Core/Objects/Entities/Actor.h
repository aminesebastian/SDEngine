#pragma once
#include "Core/Objects/Entities/Entity.h"
#include "Core/Utilities/Math/Transform.h"
#include "Core/DataStructures/DataStructures.h"
#include "Core/Utilities/EngineFunctionLibrary.h"
#include "Core/Rendering/RenderTypeDefenitions.h"

class Component;

SD_CLASS()
class Actor : public Entity {
	SD_CLASS_BODY()
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
	SD_FUNCTION()
		const Vector3D GetWorldLocation() const;
	SD_FUNCTION()
		const Vector3D GetWorldRotation() const;
	SD_FUNCTION()
		const Vector3D GetWorldScale() const;
	SD_FUNCTION()
		const FTransform GetWorldTransform() const;
	SD_FUNCTION()
		const FTransform GetLastFrameWorldTransform() const;

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

protected:
	Component* RootComponent;

private:
	SSet<Component*> Children;
};

