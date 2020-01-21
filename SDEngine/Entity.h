#pragma once
#include "Transform.h"
#include "Shader.h"
#include "World.h"
#include "DetailsPanelProvider.h"
#include "FAABB.h"

class BaseWidget;

class Entity : public IDetailsPanelProvider {

public:
	Entity(TString Name, const Transform SpawnTransform);
	Entity(TString Name);
	virtual ~Entity();

	virtual void Draw(Camera* Camera);
	virtual bool PopulateDetailsPanel() override;
	virtual TString GetDetailsPanelName() override;

	Transform GetTransform();
	Transform GetInitialTransform();
	Transform GetLastFrameTransform();

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

	void SetTransform(Transform NewTransform);

	bool IsVisible();
	void UpdateVisibility(bool bShow);
	void ToggleVisibility();
	void CompleteSpawn(UWorld* World);
	bool NeedsTick();
	bool NeedsBeginPlay();

	TString GetName();

	virtual void BeginPlay();
	virtual void Tick(float DeltaTime);
	virtual void Destroy();

	virtual void PreFrameRendered();
	virtual void PostFrameRendered();

	virtual bool TraceAgainstRay(vec3 Origin, vec3 Direction, vec3& HitPoint, float& Distance);

	FAABB* GetAxisAlignedBoundingBox();
	bool HasAxisAlignedBoundingBox();

protected:
	TString S_Name;
	const UWorld* S_World;
	Transform S_Transform;
	Transform S_LastFrameTransform;
	Transform S_InitialTransform;
	bool bVisible = true;
	bool bNeedsTick = true;
	bool bNeedsBeginPlay = true;
	FAABB* S_AABB;

	SArray<BaseWidget*> DetailsPanelWidgets;
};

