#pragma once
#include "Core/DataStructures/DataStructures.h"
#include "Core/Engine/Delegates/Event.h"
#include "Core/Engine/WindowDataTypes.h"
#include "Core/Input/InputUtilities.h"
#include "Core/Input/IUserInputReciever.h"
#include "Core/Objects/EngineObject.h"
#include "Core/Pictorum/PictorumDataTypes.h"
#include "Core/Pictorum/PictorumShapeDrawer.h"
#include "Core/Rendering/OpenGL/GPUVertexBufferArray.h"
#include "UserInterface/DetailsPanelProvider.h"

class PictorumWidget;
class Window;

class PictorumRenderer : public EngineObject, public IUserInputReciever {
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

	PictorumShapeDrawer* GetShapeDrawer() const;
	PictorumWidget* GetMouseOverWidget() const;
	const Window* GetOwningWindow() const;
	const SArray<PictorumWidget*>& GetWidgets() const;
	const PictorumWidget* GetHoveredWidget() const;

	Event<void(const vec2&)> OnMouseDownAnywhereDelegate;
	Event<void(const vec2&)> OnMouseUpAnywhereDelegate;
	Event<void(const vec2&, const vec2&)> OnMouseMoveAnywhereDelegate;
private:
	SArray<PictorumWidget*> Widgets;
	PictorumWidget* MouseOverWidget;
	PictorumShapeDrawer* ShapeDrawer;
	Window* OwningWindow;
	GPUVertexBufferArray* VertexArrayBuffer;
	bool bMouseCaptured;
	FRenderGeometry TopLevelRenderGeometry;

	void CacheMouseOverWidget(vec2 MousePosition);
	void OnWindowResized(Window* WindowIn, const FDisplayState& State);
	void GetAllInteractableWidgets(SArray<PictorumWidget*>& Widgets, PictorumWidget* Root);
};

