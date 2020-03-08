#pragma once
#include "Core/Engine/World.h"
#include "Core/Objects/CoreTypes/Shader.h"
#include "Core/Objects/EngineObject.h"
#include "Core/Physics/CollisionChannels.h"
#include "Core/Rendering/RenderTypeDefenitions.h"
#include "Core/Utilities/Logger.h"
#include "Core/Utilities/Math/Transform.h"

class Camera;
class World;

/**
 * An entity is any object that can be placed in a scene.
 * It contains all the definitions necessary to encapsulate transformation.
 * It does not contain nor does it receive gameplay events or functions.
 */
SD_CLASS()
class Entity : public EngineObject {
	SD_CLASS_BODY();
public:
	Entity(const TString& Name);
	virtual ~Entity();

	bool IsVisible() const;
	void SetVisibility(const bool& Show);
	void ToggleVisibility();

	bool IsHiddenInGame() const;
	void SetHiddenInGame(const bool& Hidden);

	bool CastsShadows() const;
	void SetCastShadows(const bool& CastShadows);

	const Vector3D& GetLocation() const;
	const Vector3D& GetRotation() const;
	const Vector3D& GetScale() const;

	const Vector3D GetLinearVelocity() const;
	const Vector3D GetAngularVelocity() const;

	void AddLocation(const Vector3D Offset);
	void AddLocation(const float& X, const float& Y, const float& Z);
	void SetLocation(const Vector3D& Location);
	void SetLocation(const float& X, const float& Y, const float& Z);

	void AddRotation(const Vector3D& RotationDelta);
	void AddRotation(const float& X, const float& Y, const float& Z);
	void SetRotation(const Vector3D& Rotation);
	void SetRotation(const float& X, const float& Y, const float& Z);

	void AddScale(const Vector3D& ScaleDelta);
	void AddScale(const float& X, const float& Y, const float& Z);
	void SetScale(const Vector3D& Scale);
	void SetScale(const float& X, const float& Y, const float& Z);
	void SetUniformScale(const float& Scale);

	const FTransform& GetTransform() const;
	void SetTransform(const FTransform& NewTransform);
	const FTransform& GetLastFrameTransform() const;
	void SetLastFrameTransform(const FTransform& OldTransform);

	void SetOwner(Entity* OwnerIn);
	Entity* GetOwner();

	//////////
	//RENDER//
	//////////
	/**
	* Draws the mesh. This method should never be overridden.
	* It makes a call to DrawAdvanced with DrawType SCENE_RENDER.
	*/
	void Draw(const Camera* RenderCamera);
	virtual void DrawAdvanced(const Camera* RenderCamera, const EDrawType& DrawType) = 0;
	bool ShouldBeDrawn(EDrawType DrawType);
	virtual void PreFrameRendered();
	virtual void PostFrameRendered();

	const class World* GetWorld() const;
protected:
	friend class World;

	SD_PROPERTY(Category=("Transform"));
	FTransform Transform;
	SD_PROPERTY(InspectorHidden);
	FTransform LastFrameTransform;

	SD_PROPERTY(Category=("Entity"));
	bool bVisible;
	SD_PROPERTY(Category=("Entity"));
	bool bHiddenInGame;
	SD_PROPERTY(Category=("Entity"));
	bool bNeedsTick;
	SD_PROPERTY(Category=("Entity"));
	bool bNeedsBeginPlay;
	SD_PROPERTY(Category=("Entity"));
	bool bCastShadows;

private:
	void OnAddedToWorld(World* WorldIn);
	void OnRemovedFromWorld();

	/** NEVER attempt to access this alone. Use the getter to that the call goes up the stack to the parent if needed. */
	World* OwningWorld;
	Entity* Owner;
};

