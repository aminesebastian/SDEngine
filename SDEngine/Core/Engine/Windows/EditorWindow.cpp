#include "EditorWindow.h"
#include "Core/Input/InputSubsystem.h"

EditorWindow::EditorWindow(const TString& Title, const vec2& Size) : Window(Title, Size) {
	_EditorWindowUIRenderer = new PictorumRenderer(Title + "PictorumRenderer", this);
	_EditorUI = nullptr;
}
EditorWindow::~EditorWindow() {

}
void EditorWindow::Initialize() {
	Engine::Get()->GetInputSubsystem()->RegisterInputReciever(_EditorWindowUIRenderer);
	_EditorUI = CreateEditorWindowUI();
	if (_EditorUI) {
		_EditorWindowUIRenderer->AddToViewport(_EditorUI);
	} else {
		SD_ENGINE_ERROR("No editor window UI provided for window: {0}. Window must override method GetEditorWindowUI().", GetTitle());
	}

	Window::Initialize();
}

void EditorWindow::UpdateInternal(const float& DeltaTime) {
	_EditorWindowUIRenderer->Tick(DeltaTime);
	_EditorWindowUIRenderer->Draw(DeltaTime);
}
PictorumWidget* EditorWindow::GetEditorWindowUI() const {
	return _EditorUI;
}