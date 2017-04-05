#pragma once
#include "Transform.h"
#include "Shader.h"
#include "World.h"

class Entity {

public:
	Entity(const Transform SpawnTransform) :
		S_Transform(SpawnTransform),
		S_InitialTransform(SpawnTransform) {}

	Entity() {}
	virtual ~Entity() {};

	virtual void Draw(Shader& shader) {}

	Transform& GetTransform() { return S_Transform; }
	Transform GetInitialTransform() { return S_InitialTransform; } 

	inline void SetTransform(Transform NewTransform) { S_Transform = NewTransform; }

	bool IsVisible() { return bVisible; }
	void UpdateVisibility(bool bShow) { bVisible = bShow; }
	void ToggleVisibility() { bVisible = !bVisible; }
	void CompleteSpawn(UWorld* World) { S_World = World; }
	bool NeedsTick() { return bNeedsTick; }
	bool NeedsBeginPlay() { return bNeedsBeginPlay; }

	virtual void BeginPlay() {}
	virtual void Tick(float DeltaTime) {}

	void Destroy() {
		delete this;
	}

protected:
	const UWorld* S_World;
	Transform S_Transform;
	Transform S_InitialTransform;
	bool bVisible = true;
	bool bNeedsTick = true;
	bool bNeedsBeginPlay = true;
};

