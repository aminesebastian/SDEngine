#include "Engine/Engine.h"
#include "PictorumRenderer.h"
#include "Core/Pictorum/PictorumWidget.h"
#include "Utilities/Logger.h"

#include "Core/Pictorum/Widgets/SolidWidget.h"
#include "Core/Pictorum/Widgets/LayoutWidget.h"
#include "Core/Pictorum/Widgets/HorizontalBoxWidget.h"
#include "Core/Pictorum/Widgets/TextWidget.h"
#include "Core/Pictorum/DistanceFieldFont.h"
#include "Core/Rendering/TextRenderer.h"

PictorumRenderer::PictorumRenderer(const TString& ViewportName, const vec2& RenderTargetResolution, const vec2& RenderTargetDPI) : EngineObject(ViewportName) {
	// Initialize member variables.
	MouseOverWidget = nullptr;
	bMouseCaptured = false;

	// Initialize render geometry.
	TopLevelRenderGeometry.SetRenderResolution(RenderTargetResolution);
	TopLevelRenderGeometry.SetAllotedSpace(RenderTargetResolution);
	TopLevelRenderGeometry.SetDPI(RenderTargetDPI);
	TopLevelRenderGeometry.SetLocation(vec2(0.0f, 0.0f));

	// Add some test widgets.
	LayoutWidget* layout = new LayoutWidget("Layout");
	HorizontalBoxWidget* hBox = new HorizontalBoxWidget("HBox");

	TextWidget* textWidget = new TextWidget("Text");
	textWidget->SetText("Hello this is a \n test of the system");
	SolidWidget* widget1 = new SolidWidget("test1");
	widget1->SetBackgroundColor(FColor(0.25f, 0.25f, 0.25f, 0.8f));


	SolidWidget* widget2 = new SolidWidget("test2");
	widget2->SetBackgroundColor(FColor(1.0f, 0.0f, 0.0f, 1.0f));
	SolidWidget* widget3 = new SolidWidget("test3");
	widget3->SetBackgroundColor(FColor(0.0f, 1.0f, 0.0f, 1.0f));
	SolidWidget* widget4 = new SolidWidget("test4");
	widget4->SetBackgroundColor(FColor(0.0f, 0.0f, 1.0f, 1.0f));

	hBox->AddChild(widget2);
	hBox->AddChild(widget3);
	hBox->AddChild(widget4);

	layout->AddChild(textWidget)->SetOffsets(1.0f, 1.0f, 0.9f, 0.0f);
	layout->AddChild(hBox)->SetOffsets(0.9f, 1.0f, 0.0f, 0.0f);

	LayoutWidgetSlot* slot = layout->AddChild(widget1);
	slot->SetOffsets(0.9f, 1.0f, 0.0f, 0.0f);
	slot->SetMargins(5, 5, 5, 5);

	AddToViewport(layout);


	//DistanceField = new DistanceFieldFont("Arial", "./Res/Fonts/Arial");
	//QuadBuffer = new TextRenderer(24, DistanceField);
}
PictorumRenderer::~PictorumRenderer() {

}

void PictorumRenderer::Tick(float DeltaTime) {
	for (PictorumWidget* widget : Widgets) {
		widget->Tick(DeltaTime, TopLevelRenderGeometry);
	}
	//QuadBuffer->SetText("The current Delta Time is: " + std::to_string(DeltaTime) + "\nSecond Line Of Text");
}
void PictorumRenderer::Draw(float DeltaTime) {
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (PictorumWidget* widget : Widgets) {
		widget->DrawContents(DeltaTime, TopLevelRenderGeometry);
	}
	//QuadBuffer->Draw(vec2(-0.5, 0.0), TopLevelRenderGeometry.GetRenderResolution(), TopLevelRenderGeometry.GetDPI());
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void PictorumRenderer::OnKeyDown(SDL_Scancode KeyCode) {

}
void PictorumRenderer::OnKeyUp(SDL_Scancode KeyCode) {

}
void PictorumRenderer::OnKeyHeld(SDL_Scancode KeyCode, float HeldTime) {

}

void PictorumRenderer::OnMouseButtonDown(vec2 ScreenPosition, EMouseButton Button) {
	if (MouseOverWidget) {
		FUserInterfaceEvent eventHandle;
		MouseOverWidget->OnMouseDown(ScreenPosition, Button, eventHandle);
		if (eventHandle.ShouldCaptureMouse()) {
			bMouseCaptured = true;
			SDL_ShowCursor(0);
		}
	}
}
void PictorumRenderer::OnMouseButtonUp(vec2 ScreenPosition, EMouseButton Button) {
	if (MouseOverWidget) {
		FUserInterfaceEvent eventHandle;
		MouseOverWidget->OnMouseUp(ScreenPosition, Button, eventHandle);
		bMouseCaptured = false;
		SDL_ShowCursor(1);
	}
}
void PictorumRenderer::OnMouseAxis(vec2 ScreenPosition, vec2 Delta) {
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

void PictorumRenderer::OnRenderTargetResolutionChanged(vec2 NewResolution) {
	TopLevelRenderGeometry.SetRenderResolution(NewResolution);
	TopLevelRenderGeometry.SetAllotedSpace(NewResolution);
	TopLevelRenderGeometry.SetLocation(vec2(0.0f, 0.0f));
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