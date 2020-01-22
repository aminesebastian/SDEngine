#include "TransformGizmo.h"
#include "Entities/StaticMesh.h"
#include "Engine/Engine.h"
#include "Engine/RenderViewport.h"
#include "Rendering/Shader.h"
#include "Utilities/Math/MathLibrary.h"
#include "Entities/Camera.h"

TransformGizmo::TransformGizmo(RenderViewport* RenderEngine) : Entity("TransformGizmo") {
	ZArrow = new StaticMesh("ZArrow", "./Res/Gizmo/LocationGizmoArrow.fbx");
	YArrow = new StaticMesh("YArrow", "./Res/Gizmo/LocationGizmoArrow.fbx");
	XArrow = new StaticMesh("XArrow", "./Res/Gizmo/LocationGizmoArrow.fbx");

	ZArrow->AddRotation(0.0f, glm::radians(-90.0f), 0.0f);
	YArrow->AddRotation(0.0f, 0.0f, glm::radians(-90.0f));

	GizmoShader = new Shader("./Res/Shaders/TransformGizmoShader", false);
	OwningEngine = RenderEngine;
	SelectedAxis = NONE;
	State = TRANSLATE;
}
TransformGizmo::~TransformGizmo() {

}
void TransformGizmo::OnMouseMove() {
	// Check to see if any axis is hovered.
	Camera* camera = Engine::GetInstance()->GetCurrentCamera();
	vec3 rayOrigin;
	vec3 rayDir;
	MathLibrary::ScreenCoordinatesToWorldRay(Engine::GetInstance()->GetMousePosition(), Engine::GetInstance()->GetDisplay()->GetDimensions(), camera->GetViewMatrix(), camera->GetProjectionMatrix(), rayOrigin, rayDir);

	vec3 hitLocation;
	float hitDistance;
	if (XArrow->TraceAgainstRay(rayOrigin, rayDir, hitLocation, hitDistance)) {
		SelectedAxis = XAXIS;
	} else if (YArrow->TraceAgainstRay(rayOrigin, rayDir, hitLocation, hitDistance)) {
		SelectedAxis = YAXIS;
	} else if (ZArrow->TraceAgainstRay(rayOrigin, rayDir, hitLocation, hitDistance)) {
		SelectedAxis = ZAXIS;
	} else {
		SelectedAxis = NONE;
	}
}
void TransformGizmo::Draw(Camera* Camera, bool bCallerProvidesShader) {
	// These values have to be set in the draw because this entity does NOT tick.
	ZArrow->SetLocation(GetLocation());
	YArrow->SetLocation(GetLocation());
	XArrow->SetLocation(GetLocation());

	float distanceToCamera = glm::length(Camera->GetLocation() - GetLocation());
	float adjustedScale = distanceToCamera / 60.0f;
	ZArrow->SetUniformScale(adjustedScale);
	YArrow->SetUniformScale(adjustedScale);
	XArrow->SetUniformScale(adjustedScale);


	// Bind the shader.
	GizmoShader->Bind();

	// Draw X Axis Arrow.
	GizmoShader->Update(XArrow->GetTransform(), Camera);
	if (SelectedAxis == XAXIS) {
		GizmoShader->SetShaderVector3("COLOR", vec3(0.92f, 0.87f, 0.20f));
	} else {
		GizmoShader->SetShaderVector3("COLOR", vec3(0.8f, 0.0f, 0.0f));
	}
	XArrow->Draw(Camera, true);

	// Draw Y Axis Arrow.
	GizmoShader->Update(YArrow->GetTransform(), Camera);
	if (SelectedAxis == YAXIS) {
		GizmoShader->SetShaderVector3("COLOR", vec3(0.92f, 0.87f, 0.20f));
	} else {
		GizmoShader->SetShaderVector3("COLOR", vec3(0.0f, 0.8f, 0.0f));
	}
	YArrow->Draw(Camera, true);

	// Draw Z Axis Arrow.
	GizmoShader->Update(ZArrow->GetTransform(), Camera);
	if (SelectedAxis == ZAXIS) {
		GizmoShader->SetShaderVector3("COLOR", vec3(0.92f, 0.87f, 0.20f));
	} else {
		GizmoShader->SetShaderVector3("COLOR", vec3(0.0f, 0.0f, 0.8f));
	}
	ZArrow->Draw(Camera, true);
}
void TransformGizmo::RecompileShaders() {
	GizmoShader->RecompileShader();
}