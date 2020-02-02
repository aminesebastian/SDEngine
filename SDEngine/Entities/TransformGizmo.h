#pragma once
#include "Entities/Entity.h"
#include "Entities/Actor.h"

class Shader;
class StaticMesh;
class RenderViewport;

class TransformGizmo : public Actor {
public:
	TransformGizmo(RenderViewport* RenderEngine);
	~TransformGizmo();

	virtual void OnMouseMove();
	virtual void DrawAdvanced(Camera* RenderCamera, EDrawType DrawType) override;

	void RecompileShaders();

	enum ESelectedAxis {
		NONE,
		XAXIS,
		YAXIS,
		ZAXIS,
		XYAXIS,
		XZAXIS,
		YZAXIS,
		XYZAXIS
	};
	enum EGizmoState {
		TRANSLATE,
		ROTATE,
		SCALE
	};
private:
	RenderViewport* OwningEngine;

	Shader* GizmoShader;
	StaticMesh* ZArrow;
	StaticMesh* YArrow;
	StaticMesh* XArrow;

	ESelectedAxis SelectedAxis;
	EGizmoState State;
};

