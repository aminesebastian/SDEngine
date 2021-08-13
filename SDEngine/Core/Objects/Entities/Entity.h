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

	SD_FUNCTION()
		bool IsVisible() const;
	SD_FUNCTION()
		void SetVisibility(const bool& Show);
	SD_FUNCTION()
		void ToggleVisibility();

	SD_FUNCTION()
		bool IsHiddenInGame() const;
	SD_FUNCTION()
		void SetHiddenInGame(const bool& Hidden);

	SD_FUNCTION()
		bool CastsShadows() const;
	SD_FUNCTION()
		void SetCastShadows(const bool& CastShadows);

	SD_FUNCTION()
		const Vector3D& GetLocation() const;
	SD_FUNCTION()
		const Vector3D& GetRotation() const;
	SD_FUNCTION()
		const Vector3D& GetScale() const;

	SD_FUNCTION()
		const Vector3D GetLinearVelocity() const;
	SD_FUNCTION()
		const Vector3D GetAngularVelocity() const;

	SD_FUNCTION()
		void AddLocation(const Vector3D Offset);
	SD_FUNCTION()
		void AddLocation(const float& X, const float& Y, const float& Z);
	SD_FUNCTION()
		void SetLocation(const Vector3D& Location);
	SD_FUNCTION()
		void SetLocation(const float& X, const float& Y, const float& Z);

	SD_FUNCTION()
		void AddRotation(const Vector3D& RotationDelta);
	SD_FUNCTION()
		void AddRotation(const float& X, const float& Y, const float& Z);
	SD_FUNCTION()
		void SetRotation(const Vector3D& Rotation);
	SD_FUNCTION()
		void SetRotation(const float& X, const float& Y, const float& Z);

	SD_FUNCTION()
		void AddScale(const Vector3D& ScaleDelta);
	SD_FUNCTION()
		void AddScale(const float& X, const float& Y, const float& Z);
	SD_FUNCTION()
		void SetScale(const Vector3D& Scale);
	SD_FUNCTION()
		void SetScale(const float& X, const float& Y, const float& Z);
	SD_FUNCTION()
		void SetUniformScale(const float& Scale);

	SD_FUNCTION()
		const FTransform& GetTransform() const;
	SD_FUNCTION()
		void SetTransform(const FTransform& NewTransform);
	SD_FUNCTION()
		const FTransform& GetLastFrameTransform() const;
	SD_FUNCTION()
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

	SD_PROPERTY(Category = ("Transform"));
	FTransform Transform;
	SD_PROPERTY(InspectorHidden);
	FTransform LastFrameTransform;

	SD_PROPERTY(Category = ("Entity"));
	bool bVisible;
	SD_PROPERTY(Category = ("Entity"));
	bool bHiddenInGame;
	SD_PROPERTY(Category = ("Entity"));
	bool bNeedsTick;
	SD_PROPERTY(Category = ("Entity"));
	bool bNeedsBeginPlay;
	SD_PROPERTY(Category = ("Entity"));
	bool bCastShadows;

private:
	void OnAddedToWorld(World* WorldIn);
	void OnRemovedFromWorld();

	/** NEVER attempt to access this alone. Use the getter to that the call goes up the stack to the parent if needed. */
	World* OwningWorld;
	Entity* Owner;
};

