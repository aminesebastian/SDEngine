#include "TransformGizmo.h"
#include "Entities/StaticMesh.h"
#include "Entities/Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/RenderViewport.h"
#include "Rendering/Shader.h"
#include "Core/Math/MathLibrary.h"
#include "Core/Input/InputSubsystem.h"
#include "Entities/Camera.h"

TransformGizmo::TransformGizmo(RenderViewport* RenderEngine) : Actor("TransformGizmo") {
	//StaticMeshComponent* xArrow = new StaticMeshComponent("XArrow");
	//StaticMeshComponent* yArrow = new StaticMeshComponent("XArrow");
	//StaticMeshComponent* zArrow = new StaticMeshComponent("XArrow");

	/*StaticMesh* mesh = new StaticMesh("TransformGizmoArrow", "./Res/Gizmo/LocationGizmoArrow.fbx");

	xArrow->SetStaticMesh(mesh);
	yArrow->SetStaticMesh(mesh);
	zArrow->SetStaticMesh(mesh);

	yArrow->AddRotation(0.0f, 0.0f, glm::radians(-90.0f));
	zArrow->AddRotation(0.0f, glm::radians(-90.0f), 0.0f);

	RegisterComponent(xArrow);
	RegisterComponent(yArrow);
	RegisterComponent(zArrow);

	GizmoShader = new Shader("./Res/Shaders/TransformGizmoShader", false);
	OwningEngine = RenderEngine;
	SelectedAxis = NONE;
	State = TRANSLATE;*/
}
TransformGizmo::~TransformGizmo() {

}
void TransformGizmo::OnMouseMove() {
	// Check to see if any axis is hovered.
	//Camera* camera = Engine::GetInstance()->GetCurrentCamera();
	//vec3 rayOrigin;
	//vec3 rayDir;
	//MathLibrary::ScreenCoordinatesToWorldRay(Engine::GetInstance()->GetInputSubsystem()->GetMousePosition(), Engine::GetInstance()->GetDisplay()->GetDimensions(), camera->GetViewMatrix(), camera->GetProjectionMatrix(), rayOrigin, rayDir);

	//vec3 hitLocation;
	//float hitDistance;
	//if (XArrow->TraceAgainstRay(rayOrigin, rayDir, hitLocation, hitDistance)) {
	//	SelectedAxis = XAXIS;
	//} else if (YArrow->TraceAgainstRay(rayOrigin, rayDir, hitLocation, hitDistance)) {
	//	SelectedAxis = YAXIS;
	//} else if (ZArrow->TraceAgainstRay(rayOrigin, rayDir, hitLocation, hitDistance)) {
	//	SelectedAxis = ZAXIS;
	//} else {
	//	SelectedAxis = NONE;
	//}
}
void TransformGizmo::DrawAdvanced(Camera* RenderCamera, EDrawType DrawType) {
	//// These values have to be set in the draw because this entity does NOT tick.
	//ZArrow->SetLocation(GetLocation());
	//YArrow->SetLocation(GetLocation());
	//XArrow->SetLocation(GetLocation());

	//float distanceToCamera = glm::length(_Camera->GetLocation() - GetLocation());
	//float adjustedScale = distanceToCamera / 60.0f;
	//ZArrow->SetUniformScale(adjustedScale);
	//YArrow->SetUniformScale(adjustedScale);
	//XArrow->SetUniformScale(adjustedScale);


	//// Bind the shader.
	//GizmoShader->Bind();

	//// Draw X Axis Arrow.
	//GizmoShader->Update(XArrow->GetTransform(), _Camera);
	//if (SelectedAxis == XAXIS) {
	//	GizmoShader->SetShaderVector3("COLOR", vec3(0.92f, 0.87f, 0.20f));
	//} else {
	//	GizmoShader->SetShaderVector3("COLOR", vec3(0.8f, 0.0f, 0.0f));
	//}
	//XArrow->Draw(_Camera, true);

	//// Draw Y Axis Arrow.
	//GizmoShader->Update(YArrow->GetTransform(), _Camera);
	//if (SelectedAxis == YAXIS) {
	//	GizmoShader->SetShaderVector3("COLOR", vec3(0.92f, 0.87f, 0.20f));
	//} else {
	//	GizmoShader->SetShaderVector3("COLOR", vec3(0.0f, 0.8f, 0.0f));
	//}
	//YArrow->Draw(_Camera, true);

	//// Draw Z Axis Arrow.
	//GizmoShader->Update(ZArrow->GetTransform(), _Camera);
	//if (SelectedAxis == ZAXIS) {
	//	GizmoShader->SetShaderVector3("COLOR", vec3(0.92f, 0.87f, 0.20f));
	//} else {
	//	GizmoShader->SetShaderVector3("COLOR", vec3(0.0f, 0.0f, 0.8f));
	//}
	//ZArrow->Draw(_Camera, true);
}
void TransformGizmo::RecompileShaders() {
	//GizmoShader->RecompileShader();
}