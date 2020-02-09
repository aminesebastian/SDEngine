#pragma once
#include "Core/Engine/Windows/EditorWindow.h"
#include "Core/Rendering/RenderViewport.h"
#include "Core/Objects/CoreTypes/Scene.h"

class MainEditorWindow : public EditorWindow {
public:
	MainEditorWindow(const vec2& Size);
	virtual ~MainEditorWindow();

	virtual void Initialize() override;
	virtual void UpdateInternal(const float& DeltaTime) override;

	void LoadScene(Scene* SceneIn);

	void OnKeyHeld(SDL_Scancode KeyCode, float HeldTime) override;
	void OnMouseAxis(vec2 ScreenPosition, vec2 Delta) override;
	void OnMouseScrollAxis(float Delta) override;
	virtual void OnKeyDown(SDL_Scancode KeyCode) override;

protected:
	RenderViewport* _RenderViewport;
	Camera* _Camera;
	World* _World;
	Scene* _CurrentScene;

	float CameraMovementSpeed;
	float CameraLookSpeed;

	virtual void WindowResized(const int32& Width, const int32& Height) override;
};

