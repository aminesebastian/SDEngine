#pragma once
#include "Core/Pictorum/PictorumWidget.h"
#include <Include\SDL\SDL_syswm.h>

class Texture2D;
class ImageWidget;
class TextWidget;

class TitleBar : public PictorumWidget {
public:
	TitleBar(const TString& Name);
	virtual ~TitleBar();
	virtual void Tick(float DeltaTime, const FRenderGeometry& Geometry) override;
	virtual void OnCreated() override;
	virtual const bool CanAddChild() const override;
	virtual void CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const override;

private:
	Texture2D* CloseTexture;
	Texture2D* MinimizeTexture;
	Texture2D* RestoreTexture;
	Texture2D* MaximizeTexture;

	ImageWidget* MinimizeButton;
	ImageWidget* MaximizeButton;
	ImageWidget* CloseButton;

	TextWidget* WindowTitleWidget;

	bool bMouseDownOnTitleBar;

	float TitleBarHeight;

	void OnCloseButtonClicked(const vec2& MousePosition, FUserInterfaceEvent& UIEvent);
	void OnMinimizeButtonClicked(const vec2& MousePosition, FUserInterfaceEvent& UIEvent);
	void OnMaximizeClicked(const vec2& MousePosition, FUserInterfaceEvent& UIEvent);

	void OnTitleBarMouseDown(const vec2& MousePosition, FUserInterfaceEvent& UIEvent);
	void OnTitleBarMouseUp(const vec2& MousePosition, FUserInterfaceEvent& UIEvent);
	void OnTitleBarMouseMove(const vec2& MousePosition, const vec2& Delta, FUserInterfaceEvent& UIEvent);

	static SDL_HitTestResult MouseHitTestCallback(SDL_Window* Window, const SDL_Point* MouseLocation, void* Data);
};

