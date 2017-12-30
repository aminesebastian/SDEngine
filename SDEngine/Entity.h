#pragma once
#include "Transform.h"
#include "Shader.h"
#include "World.h"
#include "FAABB.h"


class Entity {

public:
	Entity(TString Name, const Transform SpawnTransform) :
		S_Name(Name),
		S_Transform(SpawnTransform),
		S_InitialTransform(SpawnTransform) {}

	Entity(TString Name) : S_Name(Name){}
	virtual ~Entity() {};

	virtual void Draw(Camera* Camera) {}

	Transform& GetTransform() { return S_Transform; }
	Transform GetInitialTransform() { return S_InitialTransform; } 

	inline void SetTransform(Transform NewTransform) { S_Transform = NewTransform; }

	bool IsVisible() { return bVisible; }
	void UpdateVisibility(bool bShow) { bVisible = bShow; }
	void ToggleVisibility() { bVisible = !bVisible; }
	void CompleteSpawn(UWorld* World) { S_World = World; }
	bool NeedsTick() { return bNeedsTick; }
	bool NeedsBeginPlay() { return bNeedsBeginPlay; }

	TString GetName() { return S_Name; }

	virtual void BeginPlay() {}
	virtual void Tick(float DeltaTime) {}

	void Destroy() {
		delete this;
	}

	FAABB* GetAxisAlignedBoundingBox() { return S_AABB; }
	bool HasAxisAlignedBoundingBox() { return S_AABB != NULL; }
protected:
	TString S_Name;
	const UWorld* S_World;
	Transform S_Transform;
	Transform S_InitialTransform;
	bool bVisible = true;
	bool bNeedsTick = true;
	bool bNeedsBeginPlay = true;
	FAABB* S_AABB;
};

