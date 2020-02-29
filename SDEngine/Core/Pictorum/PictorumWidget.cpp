#include "PictorumWidget.h"
#include "Core/Engine/EngineStatics.h"
#include "Core/Engine/Engine.h"
#include "Core/Rendering/RenderViewport.h"
#include "Core/Rendering/DefferedCompositor.h"
#include "Core/Objects/CoreTypes/Shader.h"
#include "Core/Utilities/Logger.h"
#include "Core/Utilities/Math/MathLibrary.h"
#include "Core/Pictorum/Utilities/PictorumShapeDrawer.h"

PictorumWidget::PictorumWidget(const TString& Name) : EngineObject(Name, "UIWidget") {
	Rotation        = 0.0f; // 0 Degrees
	Parent          = nullptr;
	OwningRenderer  = nullptr;
	bWasClickInside = false;
	bIsBeginHovered = false;
	bFocusable      = false;
	bIsFocused      = false;

	SetVisibility(EPictorumVisibilityState::SELF_HIT_TEST_INVISIBLE);
}
PictorumWidget::~PictorumWidget() {

}

vec2 PictorumWidget::GetDesiredDrawSpace(const FRenderGeometry& Geometry) const {
	vec2 desiredSize = ZERO_VECTOR2D;
	for (PictorumWidget* child : Children) {
		vec2 childSpace = child->GetDesiredDrawSpace(Geometry);
		desiredSize.x = MathLibrary::Max(desiredSize.x, childSpace.x);
		desiredSize.y = MathLibrary::Max(desiredSize.y, childSpace.y);
	}
	return desiredSize;
}
void PictorumWidget::CalculateBounds(vec2 RenderTargetResolution, vec2& MinBounds, vec2& MaxBounds) const {
	vec2 lastLocation = LastRenderedGeometry.GetLocation(EPictorumLocationBasis::ABSOLUTE);
	vec2 lastScale = LastRenderedGeometry.GetAllotedSpace(EPictorumScaleBasis::ABSOLUTE);
	MinBounds = lastLocation;
	MaxBounds = lastLocation + lastScale;
	MinBounds.x = MathLibrary::Max(MinBounds.x, LastRenderedGeometry.GetMinimumClipPoint().x);
	MinBounds.y = MathLibrary::Max(MinBounds.y, LastRenderedGeometry.GetMinimumClipPoint().y);
	MaxBounds.x = MathLibrary::Min(MaxBounds.x, LastRenderedGeometry.GetMinimumClipPoint().x + LastRenderedGeometry.GetMaximumClipPoint().x);
	MaxBounds.y = MathLibrary::Min(MaxBounds.y, LastRenderedGeometry.GetMinimumClipPoint().y + LastRenderedGeometry.GetMaximumClipPoint().y);
}
void PictorumWidget::CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const {
	OutputGeometry = CurrentRenderGeometry;
}

IWidgetSlot* PictorumWidget::AddChild(PictorumWidget* Widget) {
	return AddChildInternal(Widget);
}
IWidgetSlot* PictorumWidget::AddChildInternal(PictorumWidget* Widget) {
	if (!CanAddChild()) {
		SD_ENGINE_WARN("There was an attempt to add a child to a widget that does not support children.");
		return nullptr;
	}
	if (Children.AddUnique(Widget)) {
		IWidgetSlot* slot = CreateSlotForWidget(Widget);
		Widget->AddedToParent(this, slot);
		OnChildAdded(Widget);
		return slot;
	}
	return nullptr;
}
const bool PictorumWidget::RemoveChild(PictorumWidget* Widget) {
	if (Children.Remove(Widget)) {
		Widget->RemovedFromParent(this);
		OnChildRemoved(Widget);
		delete Widget->GetParentSlot<IWidgetSlot>();
		delete Widget;
		return true;
	}
	return false;
}
const bool PictorumWidget::RemoveChildAt(const int32& Index) {
	if (Index > Children.LastIndex()) {
		SD_ENGINE_WARN("Index out of bounds when attempting to remove a child widget. Index: {0} for array of children of size: {1}.", Index, Children.Count());
		return false;
	}
	return RemoveChild(Children[Index]);
}
const bool PictorumWidget::CanAddChild() const {
	return true;
}
PictorumWidget* PictorumWidget::GetChildAtIndex(int32 Index) const {
	return Children[Index];
}
void PictorumWidget::ClearChildren() {
	for (int32 i = Children.LastIndex(); i >= 0; i--) {
		RemoveChildAt(i);
	}
	Children.Clear();
}
const int32 PictorumWidget::GetIndexOfChild(PictorumWidget* Widget) const {
	for (int32 i = 0; i < Children.Count(); i++) {
		if (Children[i] == Widget) {
			return i;
		}
	}
	return -1;
}
IWidgetSlot* PictorumWidget::CreateSlotForWidget(PictorumWidget* WidgetForSlot) const {
	return new IWidgetSlot();
}

const EMouseCursorStyle PictorumWidget::GetMouseCursor(const vec2& MousePosition) {
	return EMouseCursorStyle::Arrow;
}
void PictorumWidget::DrawContents(const float& DeltaTime, const FRenderGeometry& Geometry) {
	LastRenderedGeometry = Geometry;
	OnDrawStart(DeltaTime, Geometry);
	Draw(DeltaTime, Geometry);

	for (int i = 0; i < Children.Count(); i++) {
		PictorumWidget* widget = Children[i];
		FRenderGeometry childGeometry(Geometry);
		CalculateChildRenderGeometry(Geometry, childGeometry, i);
		glScissor((GLuint)childGeometry.GetMinimumClipPoint().x, (GLuint)childGeometry.GetMinimumClipPoint().y, (GLuint)childGeometry.GetMaximumClipPoint().x, (GLuint)childGeometry.GetMaximumClipPoint().y);

		widget->DrawContents(DeltaTime, childGeometry);
		OnChildDrawn(DeltaTime, Geometry, widget);
	}
	OnDrawCompleted(DeltaTime, Geometry);
	glScissor((GLuint)Geometry.GetMinimumClipPoint().x, (GLuint)Geometry.GetMinimumClipPoint().y, (GLuint)Geometry.GetMaximumClipPoint().x, (GLuint)Geometry.GetMaximumClipPoint().y);
}
void PictorumWidget::TickContents(const float& DeltaTime, const FRenderGeometry& Geometry) {
	LastRenderedGeometry = Geometry;

	Tick(DeltaTime, Geometry);

	for (int i = 0; i < Children.Count(); i++) {
		PictorumWidget* widget = Children[i];
		FRenderGeometry childGeometry(Geometry);
		CalculateChildRenderGeometry(Geometry, childGeometry, i);
		widget->TickContents(DeltaTime, childGeometry);
	}
}

void PictorumWidget::MouseEnter(const vec2& MousePosition, FUserInterfaceEvent& EventIn) {
	bIsBeginHovered = true;
	OnHoveredDelegate.Broadcast(this, MousePosition, EventIn);
	if (EventIn.ShouldContinuePropragating()) {
		OnMouseEnter(MousePosition, EventIn);
	}
}
void PictorumWidget::MouseExit(const vec2& MousePosition, FUserInterfaceEvent& EventIn) {
	if (bIsBeginHovered) {
		bIsBeginHovered = false;
		OnUnhoveredDelegate.Broadcast(this, MousePosition, EventIn);
		if (EventIn.ShouldContinuePropragating()) {
			OnMouseExit(MousePosition, EventIn);
		}
	}
}
void PictorumWidget::MouseMove(const vec2& MousePosition, const vec2& MouseDelta, FUserInterfaceEvent& EventIn) {
	OnMouseMoveDelegate.Broadcast(this, MousePosition, MouseDelta, EventIn);
	if (EventIn.ShouldContinuePropragating()) {
		OnMouseMove(MousePosition, MouseDelta, EventIn);
	}
}
void PictorumWidget::MouseDown(const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& EventIn) {
	bWasClickInside = true;
	OnMouseDownDelegate.Broadcast(this, MousePosition, Button, EventIn);
	if (EventIn.ShouldContinuePropragating()) {
		OnMouseDown(MousePosition, Button, EventIn);
	}
}
void PictorumWidget::MouseUp(const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& EventIn) {
	if (bWasClickInside) {
		bWasClickInside = false;
		OnMouseUpDelegate.Broadcast(this, MousePosition, Button, EventIn);
		if (EventIn.ShouldContinuePropragating()) {
			OnMouseUp(MousePosition, Button, EventIn);
		}
	}
}
void PictorumWidget::MouseScroll(const float Delta, FUserInterfaceEvent& EventIn) {
	OnMouseScrollDelegate.Broadcast(this, Delta, EventIn);
	if (EventIn.ShouldContinuePropragating()) {
		OnMouseScroll(Delta, EventIn);
	}
}
void PictorumWidget::KeyDown(SDL_Scancode KeyCode) {
	OnKeyDownDelegate.Broadcast(this, KeyCode);
	OnKeyDown(KeyCode);
}
void PictorumWidget::KeyUp(SDL_Scancode KeyCode) {
	OnKeyUpDelegate.Broadcast(this, KeyCode);
	OnKeyUp(KeyCode);
}
void PictorumWidget::KeyHeld(SDL_Scancode KeyCode, float HeldTime) {
	OnKeyHeldDelegate.Broadcast(this, KeyCode, HeldTime);
	OnKeyHeld(KeyCode, HeldTime);
}
void PictorumWidget::AddedToParent(PictorumWidget* ParentIn, IWidgetSlot* Slot) {
	Parent = ParentIn;
	ParentSlot = Slot;
	OnCreated();
}
void PictorumWidget::RemovedFromParent(PictorumWidget* ParentIn) {
	OnDestroyed();
}
void PictorumWidget::AddedToViewport(PictorumRenderer* Owner) {
	OwningRenderer = Owner;
	OnCreated();
	OnAddedToViewport(Owner);
}
void PictorumWidget::RemovedFromViewport() {
	OnRemovedFromViewport();
	OnDestroyed();
}
void PictorumWidget::RecievedFocus() {
	bIsFocused = true;
	OnRecievedFocusDelegate.Broadcast(this);
	OnRecievedFocus();
}
void PictorumWidget::FocusLost() {
	bIsFocused = false;
	OnFocusLostDelegate.Broadcast(this);
	OnFocusLost();
}

void PictorumWidget::SetVisibility(EPictorumVisibilityState NewVisibility) {
	Visibility = NewVisibility;
}
EPictorumVisibilityState PictorumWidget::GetVisibility() const {
	return Visibility;
}

const bool& PictorumWidget::HasFocus() const {
	return bIsFocused;
}
const float PictorumWidget::GetRenderRotation() const {
	return GetRotation() + GetParentRotation();
}
const float PictorumWidget::GetRotation() const {
	return Rotation;
}
const float PictorumWidget::GetParentRotation() const {
	if (!Parent) {
		return 0.0f;
	}
	return Parent->GetRotation();
}
void PictorumWidget::GetAllChildren(SArray<PictorumWidget*>& ChildrenOut, bool bIncludeAllDescendents) const {
	ChildrenOut.AddAll(Children);
	if (bIncludeAllDescendents) {
		for (PictorumWidget* widget : Children) {
			widget->GetAllChildren(ChildrenOut);
		}
	}
}
const int32 PictorumWidget::GetChildCount() const {
	return Children.Count();
}
PictorumWidget* PictorumWidget::GetParent() const {
	return Parent;
}
const PictorumRenderer* PictorumWidget::GetOwningRenderer() const {
	if (OwningRenderer) {
		return OwningRenderer;
	} else if (Parent) {
		return Parent->GetOwningRenderer();
	} else {
		SD_ENGINE_ERROR("Pictorum widget: {0} made a request for its owning renderer and was unable to find one up the parent chain!", GetObjectName());
		return nullptr;
	}
}
const bool& PictorumWidget::IsHovered() const {
	return bIsBeginHovered;
}
const bool& PictorumWidget::WasClickedInside() const {
	return bWasClickInside;
}

void PictorumWidget::DrawBox(const FRenderGeometry& Geometry, const FBoxDrawInstruction& Instruction) const {
	GetOwningRenderer()->GetShapeDrawer()->DrawBox(Geometry, Instruction);
}
void PictorumWidget::DrawImage(const FRenderGeometry& Geometry, const FImageDrawInstruction& Instruction) const {
	GetOwningRenderer()->GetShapeDrawer()->DrawImage(Geometry, Instruction);
}

void PictorumWidget::Tick(float DeltaTime, const FRenderGeometry& Geometry) {}
void PictorumWidget::Draw(float DeltaTime, const FRenderGeometry& Geometry) {}
void PictorumWidget::OnCreated() {}
void PictorumWidget::OnDestroyed() {}
void PictorumWidget::OnAddedToViewport(PictorumRenderer* Owner) {}
void PictorumWidget::OnRemovedFromViewport() {}
void PictorumWidget::OnAddedToParent(PictorumWidget* ParentIn, IWidgetSlot* Slot) {}
void PictorumWidget::OnRemovedFromParent(PictorumWidget* ParentIn) {}
void PictorumWidget::OnDrawStart(const float& DeltaTime, const FRenderGeometry& Geometry) {}
void PictorumWidget::OnChildDrawn(const float& DeltaTime, const FRenderGeometry& Geometry, const PictorumWidget* Child) {}
void PictorumWidget::OnDrawCompleted(const float& DeltaTime, const FRenderGeometry& Geometry) {}
void PictorumWidget::OnChildAdded(PictorumWidget* Widget) {}
void PictorumWidget::OnChildRemoved(PictorumWidget* Widget) {}
void PictorumWidget::OnMouseEnter(const vec2& MousePosition, FUserInterfaceEvent& Event) {}
void PictorumWidget::OnMouseExit(const vec2& MousePosition, FUserInterfaceEvent& Event) {}
void PictorumWidget::OnMouseMove(const vec2& MousePosition, const vec2& MouseDelta, FUserInterfaceEvent& Event) {}
void PictorumWidget::OnMouseDown(const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& Event) {}
void PictorumWidget::OnMouseUp(const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& Event) {}
void PictorumWidget::OnMouseScroll(const float Delta, FUserInterfaceEvent& EventIn) {}
void PictorumWidget::OnKeyDown(SDL_Scancode KeyCode) {}
void PictorumWidget::OnKeyUp(SDL_Scancode KeyCode) {}
void PictorumWidget::OnKeyHeld(SDL_Scancode KeyCode, float HeldTime) {}
void PictorumWidget::OnRecievedFocus() {}
void PictorumWidget::OnFocusLost() {}