#pragma once
#include "Core/Engine/Window.h"
#include "Core/Pictorum/PictorumRenderer.h"
#include "Core/Pictorum/PictorumWidget.h"

class EditorWindow : public Window {
public:
	EditorWindow(const TString& Title, const vec2& Size);
	virtual ~EditorWindow();
	virtual void Initialize() override;
	virtual void UpdateInternal(const float& DeltaTime) override;

	template<typename T>
	T* GetEditorWindowUI() const {
		return Cast<T>(_EditorUI);
	}
protected:
	PictorumRenderer* _EditorWindowUIRenderer;
	PictorumWidget* _EditorUI;

	virtual PictorumWidget* CreateEditorWindowUI() = 0;
};

