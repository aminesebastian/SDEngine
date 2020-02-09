#pragma once
#include "Core/Pictorum/PictorumWidget.h"

class Texture2D;
class ImageWidget;

class TitleBar : public PictorumWidget {
public:
	TitleBar(const TString& Name);
	virtual ~TitleBar();

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

	bool bMouseDownOnTitleBar;

	float TitleBarHeight;

	void OnCloseButtonClicked(const vec2& MousePosition, FUserInterfaceEvent& UIEvent);
	void OnMinimizeButtonClicked(const vec2& MousePosition, FUserInterfaceEvent& UIEvent);
	void OnMaximizeClicked(const vec2& MousePosition, FUserInterfaceEvent& UIEvent);

	void OnTitleBarMouseDown(const vec2& MousePosition, FUserInterfaceEvent& UIEvent);
	void OnTitleBarMouseUp(const vec2& MousePosition, FUserInterfaceEvent& UIEvent);
	void OnTitleBarMouseMove(const vec2& MousePosition, const vec2& Delta, FUserInterfaceEvent& UIEvent);

	static SDL_HitTestResult MouseHitTestCallback(SDL_Window* Window, const SDL_Point* MouseLocation, void* Data) {
		if (MouseLocation->y < 25.0f && MouseLocation->x < Engine::GetInstance()->GetDisplay()->GetDimensions().x - 75.0f) {
			return SDL_HITTEST_DRAGGABLE;
		}
		return SDL_HITTEST_NORMAL;
	}
};

