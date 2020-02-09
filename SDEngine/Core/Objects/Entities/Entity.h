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
class Entity : public EngineObject {

public:
	Entity(TString Name);
	virtual ~Entity();

	bool IsVisible() const;
	void SetVisibility(bool Show);
	void ToggleVisibility();

	bool IsHiddenInGame() const;
	void SetHiddenInGame(bool Hidden);

	bool ShouldCastShadows() const;
	void SetCastShadows(bool CastShadows);

	const vec3& GetLocation() const;
	const vec3& GetRotation() const;
	const vec3& GetScale() const;

	vec3 GetLinearVelocity() const;
	vec3 GetAngularVelocity() const;

	void AddLocation(vec3 Offset);
	void AddLocation(float X, float Y, float Z);
	void SetLocation(vec3 Location);
	void SetLocation(float X, float Y, float Z);

	void AddRotation(vec3 RotationDelta);
	void AddRotation(float X, float Y, float Z);
	void SetRotation(vec3 Rotation);
	void SetRotation(float X, float Y, float Z);

	void AddScale(vec3 ScaleDelta);
	void AddScale(float X, float Y, float Z);
	void SetScale(vec3 Scale);
	void SetScale(float X, float Y, float Z);
	void SetUniformScale(float Scale);

	const Transform& GetTransform() const;
	void SetTransform(const Transform& NewTransform);
	const Transform& GetLastFrameTransform() const;
	void SetLastFrameTransform(const Transform& OldTransform);

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

	Transform CurrentTransform;
	Transform LastFrameTrasnform;
	bool bVisible;
	bool bHiddenInGame;
	bool bNeedsTick;
	bool bNeedsBeginPlay;
	bool bCastShadows;

private:
	void OnAddedToWorld(World* WorldIn);
	void OnRemovedFromWorld();

	/** NEVER attempt to access this alone. Use the getter to that the call goes up the stack to the parent if needed. */
	World* OwningWorld;
	Entity* Owner;
};

