#include "PictorumRenderer.h"
#include "Core/Engine/Engine.h"
#include "Core/Engine/Window.h"
#include "Core/Pictorum/PictorumWidget.h"
#include "Core/Utilities/Logger.h"

PictorumRenderer::PictorumRenderer(const TString& ViewportName, Window* OwningWindow) : EngineObject(ViewportName), OwningWindow(OwningWindow) {
	ShapeDrawer   = new PictorumShapeDrawer(ViewportName + "_ShapeDrawer");
	FocusedWidget = nullptr;

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
	CacheMouseOverWidgets(Engine::GetInputSubsystem()->GetMousePosition());

	// Raise mouse exit events.
	for (int32 i = LastFrameWidgetsUnderMouse.LastIndex(); i >= 0; i--) {
		PictorumWidget* mouseOver = LastFrameWidgetsUnderMouse[i];
		if (!WidgetsUnderMouse.Contains(mouseOver)) {
			FUserInterfaceEvent eventHandle;
			mouseOver->MouseExit(Engine::GetInputSubsystem()->GetMousePosition(), eventHandle);
			LastFrameWidgetsUnderMouse.Remove(mouseOver);
		}
	}

	// Raise mouse enter events.
	for (int32 i = 0; i < WidgetsUnderMouse.Count(); i++) {
		PictorumWidget* candidate = WidgetsUnderMouse[i];
		if (!LastFrameWidgetsUnderMouse.Contains(candidate) && WidgetsUnderMouse.Contains(candidate)) {
			FUserInterfaceEvent eventHandle;
			candidate->MouseEnter(Engine::GetInputSubsystem()->GetMousePosition(), eventHandle);
		}
	}

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
	if (FocusedWidget) {
		FocusedWidget->KeyDown(KeyCode);
	}
}
void PictorumRenderer::OnKeyUp(SDL_Scancode KeyCode) {
	if (FocusedWidget) {
		FocusedWidget->KeyUp(KeyCode);
	}
}
void PictorumRenderer::OnKeyHeld(SDL_Scancode KeyCode, float HeldTime) {
	if (FocusedWidget) {
		FocusedWidget->KeyHeld(KeyCode, HeldTime);
	}
}
void PictorumRenderer::OnTextInput(const TString& Input) {
	if (FocusedWidget) {
		FocusedWidget->OnTextInput(Input);
	}
}
void PictorumRenderer::OnMouseButtonDown(vec2 ScreenPosition, EMouseButton Button) {
	OnMouseDownAnywhereDelegate.Broadcast(ScreenPosition);

	bool focusChanged = false;
	bool isFocusableValid = false;
	FUserInterfaceEvent eventHandle;
	for (PictorumWidget* candidate : WidgetsUnderMouse) {
		if (!focusChanged) {
			if (candidate->bFocusable) {
				if (candidate != FocusedWidget) {
					SetFocusedWidget(candidate);
					focusChanged = true;
				}
				isFocusableValid = true;
			}
		}

		candidate->MouseDown(ScreenPosition, Button, eventHandle);
		if (eventHandle.bCaptureMouse) {
			Engine::GetInputSubsystem()->CaptureMouseCursor();
			Engine::GetInputSubsystem()->HideMouseCursor();
		}
		if (!eventHandle.ShouldContinuePropragating()) {
			break;
		}
	}

	if (!isFocusableValid) {
		ClearFocusedWidget();
	}
}
void PictorumRenderer::OnMouseButtonUp(vec2 ScreenPosition, EMouseButton Button) {
	OnMouseUpAnywhereDelegate.Broadcast(ScreenPosition);

	FUserInterfaceEvent eventHandle;
	for (PictorumWidget* candidate : WidgetsUnderMouse) {
		candidate->MouseUp(ScreenPosition, Button, eventHandle);
		Engine::GetInputSubsystem()->ReleaseMouseCursor();
		Engine::GetInputSubsystem()->ShowMouseCursor();
		if (!eventHandle.ShouldContinuePropragating()) {
			break;
		}
	}
}
void PictorumRenderer::OnMouseAxis(vec2 ScreenPosition, vec2 Delta) {
	OnMouseMoveAnywhereDelegate.Broadcast(ScreenPosition, Delta);

	bool wasCursorOverriden = false;

	// Raise mouse move events.
	FUserInterfaceEvent eventHandle;
	for (PictorumWidget* candidate : WidgetsUnderMouse) {
		candidate->MouseMove(ScreenPosition, Delta, eventHandle);
		bool overrideCursor = false;
		EMouseCursorStyle cursor = candidate->GetMouseCursor(ScreenPosition, overrideCursor);
		if (overrideCursor) {
			wasCursorOverriden = true;
			Engine::GetInputSubsystem()->SetMouseCursorStyle(cursor);
		}
	}

	if (!wasCursorOverriden) {
		Engine::GetInputSubsystem()->SetMouseCursorStyle(EMouseCursorStyle::Arrow);
	}
}
void PictorumRenderer::OnMouseScrollAxis(float Delta) {
	OnMouseScrollAnywhereDelegate.Broadcast(Delta);

	FUserInterfaceEvent eventHandle;
	for (PictorumWidget* candidate : WidgetsUnderMouse) {
		candidate->MouseScroll(Delta, eventHandle);
		if (!eventHandle.ShouldContinuePropragating()) {
			break;
		}
	}
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

PictorumShapeDrawer* PictorumRenderer::GetShapeDrawer() const {
	return ShapeDrawer;
}
void PictorumRenderer::CacheMouseOverWidgets(const Vector2D& MousePosition) {
	if (Engine::GetInputSubsystem()->IsMouseCursorCaptured()) {
		return;
	}

	// Capture the widgets that were under the mouse the previous frame. This is used to detect
	// when widgets have just been hovered or unhovered.
	for (PictorumWidget* lastFrame : WidgetsUnderMouse) {
		LastFrameWidgetsUnderMouse.AddUnique(lastFrame);
	}

	// Clear the widgets under the mouse.
	WidgetsUnderMouse.Clear();

	// Capture all the widgets that can be hit.
	SArray <PictorumWidget*> interactableChildren;
	for (PictorumWidget* widget : Widgets) {
		GetAllInteractableWidgets(interactableChildren, widget);
	}

	// Test them each for hits.
	for (PictorumWidget* child : interactableChildren) {
		vec2 min, max;
		child->CalculateBounds(OwningWindow->GetDimensions(), min, max);

		// Add all widgets under the cursor to the list of widgets.
		if (MousePosition.x >= min.x && MousePosition.x <= max.x && MousePosition.y >= min.y && MousePosition.y <= max.y) {
			WidgetsUnderMouse.AddUnique(child);
		}
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
const SArray<PictorumWidget*>& PictorumRenderer::GetWidgetsUnderCursor() const {
	return WidgetsUnderMouse;
}
const Window* PictorumRenderer::GetOwningWindow() const {
	return OwningWindow;
}
const SArray<PictorumWidget*>& PictorumRenderer::GetWidgets() const {
	return Widgets;
}
void PictorumRenderer::OnWindowResized(Window* WindowIn, const FDisplayState& State) {
	TopLevelRenderGeometry.SetRenderResolution(State.GetResolution());
	TopLevelRenderGeometry.SetAllotedSpace(State.GetResolution());
	TopLevelRenderGeometry.SetLocation(vec2(0.0f, 0.0f));
	TopLevelRenderGeometry.SetMaximumClipPoint(State.GetResolution());
}
void PictorumRenderer::SetFocusedWidget(PictorumWidget* Widget) {
	ClearFocusedWidget();
	FocusedWidget = Widget;
	Widget->RecievedFocus();
}
void PictorumRenderer::ClearFocusedWidget() {
	if(FocusedWidget) {
		FocusedWidget->FocusLost();
		FocusedWidget = nullptr;
	}
}