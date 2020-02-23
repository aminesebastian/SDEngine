#include "MainEditorWindow.h"
#include "Core/Engine/Engine.h"
#include "Core/Engine/World.h"
#include "Core/Input/InputSubsystem.h"
#include "Core/Objects/Entities/Camera.h"
#include "Core/Pictorum/EngineUI/MainViewportUserInterface.h"
#include "GLM/glm.hpp"

MainEditorWindow::MainEditorWindow(const vec2& Size) : EditorWindow("MainEditorViewport", Size) {
	_World = new World();
	_RenderViewport = new RenderViewport(_World, this);
	CameraMovementSpeed = 300.0f;
	CameraLookSpeed = 200.0f;
	_CurrentScene = nullptr;

	Transform cameraTransform;
	cameraTransform.SetRotation(0, glm::radians(50.0f), glm::radians(-180.0f));
	cameraTransform.AddLocation(vec3(35.0f, 0.0f, 35.0f));
	_Camera = new Camera("MainEditorWindowCamera", cameraTransform, radians(50.0f), GetDimensions(), 0.01f, 100000.0f);

	_World->RegisterActor(_Camera);
}
MainEditorWindow::~MainEditorWindow() {

}
void MainEditorWindow::Initialize() {
	_RenderViewport->Initialize();

	Scene* scene = new Scene();
	LoadScene(scene);

	EditorWindow::Initialize();

	GetEditorWindowUI<MainViewportUserInterface>()->OnWorldUpdated(_World);
}
void MainEditorWindow::UpdateInternal(const float& DeltaTime) {
	_World->TickWorld(DeltaTime);
	_RenderViewport->Render(_Camera);
	EditorWindow::UpdateInternal(DeltaTime);
}
void MainEditorWindow::LoadScene(Scene* SceneIn) {
	if (_CurrentScene) {
		_CurrentScene->SaveScene();
		_CurrentScene->UnloadScene(_World);
		delete _CurrentScene;
	}
	if (SceneIn->LoadScene(_World)) {
		_CurrentScene = SceneIn;
	}
}
PictorumWidget* MainEditorWindow::CreateEditorWindowUI() {
	return new MainViewportUserInterface("EngineUI");
}
void MainEditorWindow::WindowResized(const int32& Width, const int32& Height) {
	EditorWindow::WindowResized(Width, Height);
	_Camera->SetRenderTargetDimensions(GetDisplayState().GetResolution());
}
void MainEditorWindow::OnKeyHeld(SDL_Scancode KeyCode, float HeldTime) {
	float alpha = CameraMovementSpeed * (float)GetFrameTime();
	if (KeyCode == SDL_SCANCODE_W) {
		_Camera->AddLocation(_Camera->GetTransform().GetForwardVector() * alpha);
	}
	if (KeyCode == SDL_SCANCODE_S) {
		_Camera->AddLocation(-_Camera->GetTransform().GetForwardVector() * alpha);
	}
	if (KeyCode == SDL_SCANCODE_A) {
		_Camera->AddLocation(_Camera->GetTransform().GetRightVector() * alpha);
	}
	if (KeyCode == SDL_SCANCODE_D) {
		_Camera->AddLocation(-_Camera->GetTransform().GetRightVector() * alpha);
	}
	if (KeyCode == SDL_SCANCODE_Q) {
		_Camera->AddLocation(-_Camera->GetTransform().GetUpVector() * alpha);
	}
	if (KeyCode == SDL_SCANCODE_E) {
		_Camera->AddLocation(_Camera->GetTransform().GetUpVector() * alpha);
	}
}
void MainEditorWindow::OnKeyDown(SDL_Scancode KeyCode) {
	if (KeyCode == SDL_SCANCODE_G) {
		_World->SetGameMode(!_World->IsInGameMode());
	}
}
void MainEditorWindow::OnMouseAxis(vec2 ScreenPosition, vec2 Delta) {
	if (Engine::GetInputSubsystem()->IsMouseButtonDown(EMouseButton::RIGHT)) {
		//Engine::GetInputSubsystem()->HideMouseCursor();
		//Engine::GetInputSubsystem()->CaptureMouseCursor();
		_Camera->AddOrbit((float)(-Delta.y) / CameraLookSpeed, -(float)(Delta.x) / CameraLookSpeed);
	} else if (Engine::GetInputSubsystem()->IsMouseButtonDown(EMouseButton::MIDDLE)) {
		//Engine::GetInputSubsystem()->HideMouseCursor();
		//Engine::GetInputSubsystem()->CaptureMouseCursor();
		_Camera->AddLocation(-_Camera->GetTransform().GetRightVector() * (float)(Delta.x) / 250.0f);
		_Camera->AddLocation(_Camera->GetTransform().GetUpVector() * (float)(Delta.y) / 250.0f);
	}
}
void MainEditorWindow::OnMouseScrollAxis(float Delta) {
	CameraMovementSpeed = glm::clamp(CameraMovementSpeed + ((float)Delta * GetFrameTime() * 1000.0f), 1.0f, 1000.0f);
}