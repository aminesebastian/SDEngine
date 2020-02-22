#include "PictorumRenderer.h"
#include "Core/Engine/Engine.h"
#include "Core/Engine/Window.h"
#include "Core/Pictorum/PictorumWidget.h"
#include "Core/Pictorum/EngineUI/EngineUIContainer.h"
#include "Core/Utilities/Logger.h"

PictorumRenderer::PictorumRenderer(const TString& ViewportName, Window* OwningWindow) : EngineObject(ViewportName), OwningWindow(OwningWindow) {
	// Initialize member variables.
	MouseOverWidget = nullptr;
	bMouseCaptured = false;

	ShapeDrawer = new PictorumShapeDrawer(ViewportName + "_ShapeDrawer");

	// Initialize render geometry.
	TopLevelRenderGeometry.SetRenderResolution(OwningWindow->GetDimensions());
	TopLevelRenderGeometry.SetAllotedSpace(OwningWindow->GetDimensions());
	TopLevelRenderGeometry.SetDPI(OwningWindow->GetDisplayDPI());
	TopLevelRenderGeometry.SetLocation(vec2(0.0f, 0.0f));
	TopLevelRenderGeometry.SetMaximumClipPoint(OwningWindow->GetDimensions());

	OwningWindow->OnWindowResized.Add<PictorumRenderer, & PictorumRenderer::OnWindowResized>(this);

	VertexArrayBuffer = new GPUVertexBufferArray();

	SArray<Vector3D> verticies;
	verticies.Emplace(0.0f, 1.0f, 0.0f);
	verticies.Emplace(0.0f, 0.0f, 0.0f);
	verticies.Emplace(1.0f, 1.0f, 0.0f);
	verticies.Emplace(1.0f, 0.0f, 0.0f);

	VertexArrayBuffer->AddBuffer("Vertex", EGPUBufferType::ArrayBuffer, EGPUBufferUsage::StaticDraw, EGPUBufferDataType::Float);
	VertexArrayBuffer->SetBufferData(0, verticies, verticies.Count());
	VertexArrayBuffer->Update();
}
PictorumRenderer::~PictorumRenderer() {

}

void PictorumRenderer::Tick(float DeltaTime) {
	for (PictorumWidget* widget : Widgets) {
		widget->TickContents(DeltaTime, TopLevelRenderGeometry);
	}
}
void PictorumRenderer::Draw(float DeltaTime) {
	glEnable(GL_BLEND);
	glEnable(GL_SCISSOR_TEST);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (PictorumWidget* widget : Widgets) {
		glScissor((GLuint)TopLevelRenderGeometry.GetMinimumClipPoint().x, (GLuint)TopLevelRenderGeometry.GetMinimumClipPoint().y, (GLuint)TopLevelRenderGeometry.GetMaximumClipPoint().x, (GLuint)TopLevelRenderGeometry.GetMaximumClipPoint().y);
		widget->DrawContents(DeltaTime, TopLevelRenderGeometry);
	}
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_BLEND);
}

void PictorumRenderer::OnKeyDown(SDL_Scancode KeyCode) {

}
void PictorumRenderer::OnKeyUp(SDL_Scancode KeyCode) {

}
void PictorumRenderer::OnKeyHeld(SDL_Scancode KeyCode, float HeldTime) {

}

void PictorumRenderer::OnMouseButtonDown(vec2 ScreenPosition, EMouseButton Button) {
	OnMouseDownAnywhereDelegate.Broadcast(ScreenPosition);

	if (MouseOverWidget) {
		FUserInterfaceEvent eventHandle;
		MouseOverWidget->MouseDown(ScreenPosition, Button, eventHandle);
		if (eventHandle.ShouldCaptureMouse()) {
			bMouseCaptured = true;
			SDL_CaptureMouse(SDL_TRUE);
			SDL_ShowCursor(0);
		}
	}
}
void PictorumRenderer::OnMouseButtonUp(vec2 ScreenPosition, EMouseButton Button) {
	OnMouseUpAnywhereDelegate.Broadcast(ScreenPosition);

	if (MouseOverWidget) {
		FUserInterfaceEvent eventHandle;
		MouseOverWidget->MouseUp(ScreenPosition, Button, eventHandle);
		bMouseCaptured = false;
		SDL_CaptureMouse(SDL_FALSE);
		SDL_ShowCursor(1);
	}
}
void PictorumRenderer::OnMouseAxis(vec2 ScreenPosition, vec2 Delta) {
	PictorumWidget* previousMouseOver = GetMouseOverWidget();
	CacheMouseOverWidget(ScreenPosition);

	OnMouseMoveAnywhereDelegate.Broadcast(ScreenPosition, Delta);

	if (MouseOverWidget) {
		Engine::GetInputSubsystem()->SetMouseCursorStyle(MouseOverWidget->GetMouseCursor(ScreenPosition));
	} else {
		Engine::GetInputSubsystem()->SetMouseCursorStyle(EMouseCursorStyle::Arrow);
	}

	// Raise mouse enter and exit events.
	if (MouseOverWidget != previousMouseOver) {
		if (previousMouseOver) {
			FUserInterfaceEvent eventHandle;
			previousMouseOver->MouseExit(ScreenPosition, eventHandle);
		}
		if (MouseOverWidget) {
			FUserInterfaceEvent eventHandle;
			MouseOverWidget->MouseEnter(ScreenPosition, eventHandle);
		}
	}

	// Raise mouse move events.
	if (MouseOverWidget) {
		FUserInterfaceEvent eventHandle;
		MouseOverWidget->MouseMove(ScreenPosition, Delta, eventHandle);
	}
}
void PictorumRenderer::OnMouseScrollAxis(float Delta) {
	OnMouseScrollAnywhereDelegate.Broadcast(Delta);

	if (MouseOverWidget) {
		FUserInterfaceEvent eventHandle;
		MouseOverWidget->MouseScroll(Delta, eventHandle);
		bMouseCaptured = false;
		SDL_CaptureMouse(SDL_FALSE);
		SDL_ShowCursor(1);
	}
}

PictorumWidget* PictorumRenderer::GetMouseOverWidget() const {
	return MouseOverWidget;
}

PictorumShapeDrawer* PictorumRenderer::GetShapeDrawer() const {
	return ShapeDrawer;
}
bool PictorumRenderer::AddToViewport(PictorumWidget* Widget) {
	if (!Widget) {
		return false;
	}
	if (Widgets.AddUnique(Widget)) {
		Widget->AddedToViewport(this);
		return true;
	}
	return false;
}
bool PictorumRenderer::RemoveFromViewport(PictorumWidget* Widget) {
	if (!Widget) {
		return false;
	}
	if (Widgets.Remove(Widget)) {
		Widget->RemovedFromViewport();
		return true;
	}
	return false;
}
void PictorumRenderer::CacheMouseOverWidget(vec2 MousePosition) {
	if (bMouseCaptured) {
		return;
	}

	// Capture all the widgets that can be hit.
	SArray <PictorumWidget*> interactableChildren;
	for (PictorumWidget* widget : Widgets) {
		GetAllInteractableWidgets(interactableChildren, widget);
	}

	bool hit = false;

	// Test them each for hits.
	for (PictorumWidget* child : interactableChildren) {
		vec2 min, max;
		child->CalculateBounds(OwningWindow->GetDimensions(), min, max);

		// Keep looping, the later the overlap, the closer the widget is to the top of the stack.
		if (MousePosition.x >= min.x && MousePosition.x <= max.x && MousePosition.y >= min.y && MousePosition.y <= max.y) {
			hit = true;
			MouseOverWidget = child;
		}
	}
	if (!hit) {
		MouseOverWidget = nullptr;
	}
}
void PictorumRenderer::GetAllInteractableWidgets(SArray<PictorumWidget*>& Widgets, PictorumWidget* Root) {
	if (!Root) {
		return;
	}
	if (Root->GetVisibility() == EPictorumVisibilityState::COLLAPSED || Root->GetVisibility() == EPictorumVisibilityState::HIDDEN || Root->GetVisibility() == EPictorumVisibilityState::HIT_TEST_INVISIBLE) {
		return;
	}
	if (Root->GetVisibility() == EPictorumVisibilityState::VISIBLE) {
		Widgets.Add(Root);
	}
	SArray<PictorumWidget*> children;
	Root->GetAllChildren(children);
	for (PictorumWidget* child : children) {
		GetAllInteractableWidgets(Widgets, child);
	}
}
const Window* PictorumRenderer::GetOwningWindow() const {
	return OwningWindow;
}
const SArray<PictorumWidget*>& PictorumRenderer::GetWidgets() const {
	return Widgets;
}
const PictorumWidget* PictorumRenderer::GetHoveredWidget() const {
	return MouseOverWidget;
}
void PictorumRenderer::OnWindowResized(Window* WindowIn, const FDisplayState& State) {
	TopLevelRenderGeometry.SetRenderResolution(State.GetResolution());
	TopLevelRenderGeometry.SetAllotedSpace(State.GetResolution());
	TopLevelRenderGeometry.SetLocation(vec2(0.0f, 0.0f));
	TopLevelRenderGeometry.SetMaximumClipPoint(State.GetResolution());
}
