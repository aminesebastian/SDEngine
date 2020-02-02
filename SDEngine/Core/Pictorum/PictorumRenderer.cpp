#include "Engine/Engine.h"
#include "PictorumRenderer.h"
#include "Core/Pictorum/PictorumWidget.h"
#include "Utilities/Logger.h"

#include "Core/Pictorum/Widgets/SolidWidget.h"
#include "Core/Pictorum/Widgets/LayoutWidget.h"

PictorumRenderer::PictorumRenderer(TString ViewportName) : EngineObject(ViewportName) {
	MouseOverWidget = nullptr;
	bMouseCaptured  = false;

	LayoutWidget* layout = new LayoutWidget("Layout");
	layout->AddChild(new SolidWidget("test2"));
	AddToViewport(layout);
}
PictorumRenderer::~PictorumRenderer() {

}

void PictorumRenderer::Tick(float DeltaTime) {
	FRenderGeometry geometry;
	geometry.RenderResolution = Engine::GetInstance()->GetFocusedViewport()->GetRenderTargetDimensions();
	for (PictorumWidget* widget : Widgets) {
		widget->Tick(DeltaTime, geometry);
	}
}
void PictorumRenderer::Draw(float DeltaTime) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	FRenderGeometry geometry;
	geometry.RenderResolution = Engine::GetInstance()->GetFocusedViewport()->GetRenderTargetDimensions();
	for (PictorumWidget* widget : Widgets) {
		widget->DrawContents(DeltaTime, geometry);
	}
	glDisable(GL_BLEND);
}

void PictorumRenderer::OnKeyDown(SDL_Scancode KeyCode){ 

}
void PictorumRenderer::OnKeyUp(SDL_Scancode KeyCode){ 

}
void PictorumRenderer::OnKeyHeld(SDL_Scancode KeyCode, float HeldTime){ 

}

void PictorumRenderer::OnMouseButtonDown(vec2 ScreenPosition, EMouseButton Button){ 
	if (MouseOverWidget) {
		FUserInterfaceEvent eventHandle;
		MouseOverWidget->OnMouseDown(ScreenPosition, Button, eventHandle);
		if (eventHandle.ShouldCaptureMouse()) {
			bMouseCaptured = true;
			SDL_ShowCursor(0);
		}
	}
}
void PictorumRenderer::OnMouseButtonUp(vec2 ScreenPosition, EMouseButton Button){ 
	if (MouseOverWidget) {
		FUserInterfaceEvent eventHandle;
		MouseOverWidget->OnMouseUp(ScreenPosition, Button, eventHandle);
		bMouseCaptured = false;
		SDL_ShowCursor(1);
	}
}
void PictorumRenderer::OnMouseAxis(vec2 ScreenPosition, vec2 Delta){ 
	PictorumWidget* previousMouseOver = GetMouseOverWidget();
	CacheMouseOverWidget(ScreenPosition);

	// Raise mouse enter and exit events.
	if (MouseOverWidget != previousMouseOver) {
		if (previousMouseOver) {
			FUserInterfaceEvent eventHandle;
			previousMouseOver->OnMouseExit(ScreenPosition, eventHandle);
		}
		if (MouseOverWidget) {
			FUserInterfaceEvent eventHandle;
			MouseOverWidget->OnMouseEnter(ScreenPosition, eventHandle);
		}
	}

	// Raise mouse move events.
	if (MouseOverWidget) {
		FUserInterfaceEvent eventHandle;
		MouseOverWidget->OnMouseMove(ScreenPosition, Delta, eventHandle);
	}
}
void PictorumRenderer::OnMouseScrollAxis(float Delta) {

}

PictorumWidget* PictorumRenderer::GetMouseOverWidget() {
	return MouseOverWidget;
}

bool PictorumRenderer::AddToViewport(PictorumWidget* Widget) {
	if (!Widget) {
		return false;
	}
	return Widgets.AddUnique(Widget);
}
bool PictorumRenderer::RemoveFromViewport(PictorumWidget* Widget) {
	if (!Widget) {
		return false;
	}
	return Widgets.Remove(Widget);
}
void PictorumRenderer::CacheMouseOverWidget(vec2 MousePosition) {
	if (bMouseCaptured) {
		return;
	}

	// Capture all the widgets that can be hit.
	SArray <PictorumWidget*> children;
	for (PictorumWidget* widget : Widgets) {
		if (widget->GetVisibility() == EPictorumVisibilityState::VISIBLE) {
			children.Add(widget);
			widget->GetAllChildren(children);
		} else if (widget->GetVisibility() == EPictorumVisibilityState::SELF_HIT_TEST_INVISIBLE) {
			widget->GetAllChildren(children);
		}
	}

	// Test them each for hits.
	for (PictorumWidget* child : children) {
		if (child->GetVisibility() != EPictorumVisibilityState::VISIBLE) {
			continue;
		}
		vec2 min, max;
		child->CalculateBounds(Engine::GetInstance()->GetFocusedViewport()->GetRenderTargetDimensions(), min, max);

		if (MousePosition.x > min.x && MousePosition.x < max.x && MousePosition.y > min.y && MousePosition.y < max.y) {
			MouseOverWidget = child;
			return;
		}
	}
	MouseOverWidget = nullptr;
}
const SArray<PictorumWidget*>& PictorumRenderer::GetWidgets() {
	return Widgets;
}
TString PictorumRenderer::GetDetailsPanelName() {
	return "UI Widget";
}
bool PictorumRenderer::PopulateDetailsPanel() {
	if (Widgets[0]) {
		Widgets[0]->PopulateDetailsPanel();
	}
	return true;
}