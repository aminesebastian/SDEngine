#pragma once
#include "Core/DataStructures/DataStructures.h"
#include "Core/Engine/WindowDataTypes.h"
#include "Core/Input/InputUtilities.h"
#include "Core/Input/IUserInputReciever.h"
#include "Core/Objects/EngineObject.h"
#include "Core/Pictorum/PictorumDataTypes.h"
#include "Core/Engine/Delegates/Event.h"
#include "UserInterface/DetailsPanelProvider.h"

class PictorumWidget;
class TextRenderer;
class DistanceFieldFont;
class Window;

class PictorumRenderer : public EngineObject, public IUserInputReciever, public IDetailsPanelProvider {
public:
	PictorumRenderer(const TString& ViewportName, Window* OwningWindow);
	virtual ~PictorumRenderer();

	void Tick(float DeltaTime);
	void Draw(float DeltaTime);

	virtual void OnKeyDown(SDL_Scancode KeyCode) override;
	virtual void OnKeyUp(SDL_Scancode KeyCode) override;
	virtual void OnKeyHeld(SDL_Scancode KeyCode, float HeldTime) override;

	virtual void OnMouseButtonDown(vec2 ScreenPosition, EMouseButton Button) override;
	virtual void OnMouseButtonUp(vec2 ScreenPosition, EMouseButton Button) override;
	virtual void OnMouseAxis(vec2 ScreenPosition, vec2 Delta) override;
	virtual void OnMouseScrollAxis(float Delta) override;

	bool AddToViewport(PictorumWidget* Widget);
	bool RemoveFromViewport(PictorumWidget* Widget);

	PictorumWidget* GetMouseOverWidget();
	const Window* GetOwningWindow() const;
	const SArray<PictorumWidget*>& GetWidgets() const;
	const PictorumWidget* GetHoveredWidget() const;

	virtual TString GetDetailsPanelName() override;
	virtual bool PopulateDetailsPanel() override;

	Event<void(const vec2&)> OnMouseDownAnywhereDelegate;
	Event<void(const vec2&)> OnMouseUpAnywhereDelegate;
	Event<void(const vec2&, const vec2&)> OnMouseMoveAnywhereDelegate;
private:
	SArray<PictorumWidget*> Widgets;
	PictorumWidget* MouseOverWidget;
	Window* OwningWindow;

	bool bMouseCaptured;
	FRenderGeometry TopLevelRenderGeometry;

	void CacheMouseOverWidget(vec2 MousePosition);
	void OnWindowResized(Window* WindowIn, const FDisplayState& State);
	void GetAllInteractableWidgets(SArray<PictorumWidget*>& Widgets, PictorumWidget* Root);
};

