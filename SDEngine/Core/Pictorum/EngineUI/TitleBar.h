#pragma once
#include "Core/Pictorum/PictorumWidget.h"
#include "Core/Engine/WindowDataTypes.h"
#include <Include\SDL\SDL_syswm.h>

class Texture2D;
class ImageWidget;
class TextWidget;
class LayoutWidget;

class TitleBar : public PictorumWidget {
public:
	TitleBar(const TString& Name);
	virtual ~TitleBar();
	virtual void Tick(float DeltaTime, const FRenderGeometry& Geometry) override;
	virtual void OnCreated() override;
	virtual const bool CanAddChild() const override;

private:
	Texture2D* CloseTexture;
	Texture2D* MinimizeTexture;
	Texture2D* RestoreTexture;
	Texture2D* MaximizeTexture;

	ImageWidget* MinimizeButton;
	ImageWidget* MaximizeButton;
	ImageWidget* CloseButton;

	LayoutWidget* MasterContainer;
	TextWidget* WindowTitleWidget;

	bool bMouseDownOnTitleBar;

	float TitleBarHeight;

	void OnCloseButtonClicked(const vec2& MousePosition, FUserInterfaceEvent& UIEvent);
	void OnMinimizeButtonClicked(const vec2& MousePosition, FUserInterfaceEvent& UIEvent);
	void OnMaximizeClicked(const vec2& MousePosition, FUserInterfaceEvent& UIEvent);

	void OnWindowMaximized(const int32& WindowId, const FDisplayState& State);
	void OnWindowRestored(const int32& WindowId, const FDisplayState& State);

	static SDL_HitTestResult MouseHitTestCallback(SDL_Window* Window, const SDL_Point* MouseLocation, void* Data);
};

