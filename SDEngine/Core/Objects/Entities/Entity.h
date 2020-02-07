#pragma once
#include "Core/Objects/EngineObject.h"
#include "Core/Physics/AxisAlignedBoundingBox.h"
#include "Core/Physics/CollisionChannels.h"
#include "Core/Objects/CoreTypes/Shader.h"
#include "Core/Utilities/Logger.h"
#include "Core/Utilities/Math/Transform.h"
#include "Core/Rendering/RenderTypeDefenitions.h"

class Camera;

/**
 * An entity is any object that can be placed in a scene.
 * It contains all the definitions necessary to encapsulate a transformation.
 * It does not contain nor does it receive gameplay events or functions.
 */
class Entity : public EngineObject {

public:
	Entity(TString Name);
	virtual ~Entity();

	bool IsVisible();
	void SetVisibility(bool Show);
	void ToggleVisibility();

	bool IsHiddenInGame();
	void SetHiddenInGame(bool Hidden);

	bool ShouldCastShadows();
	void SetCastShadows(bool CastShadows);

	bool ShouldBeDrawn(EDrawType DrawType);

	vec3 GetLocation();
	vec3 GetRotation();
	vec3 GetScale();

	vec3 GetLinearVelocity();
	vec3 GetAngularVelocity();

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

	Transform GetTransform();
	void SetTransform(Transform NewTransform);
	Transform GetLastFrameTransform();
	void SetLastFrameTransform(Transform OldTransform);

	//////////
	//RENDER//
	//////////
	/**
	* Draws the mesh. This method should never be overridden.
	* It makes a call to DrawAdvanced with DrawType SCENE_RENDER.
	*/
	void Draw(Camera* RenderCamera);
	virtual void DrawAdvanced(Camera* RenderCamera, EDrawType DrawType) = 0;
	virtual void PreFrameRendered();
	virtual void PostFrameRendered();

protected:
	Transform CurrentTransform;
	Transform LastFrameTrasnform;
	bool bVisible;
	bool bHiddenInGame;
	bool bNeedsTick;
	bool bNeedsBeginPlay;
	bool bCastShadows;
};

