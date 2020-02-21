#include "PictorumWidget.h"
#include "Core/Engine/EngineStatics.h"
#include "Core/Engine/Engine.h"
#include "Core/Rendering/RenderViewport.h"
#include "Core/Rendering/DefferedCompositor.h"
#include "Core/Objects/CoreTypes/Shader.h"
#include "Core/Utilities/Logger.h"
#include "Core/Utilities/Math/MathLibrary.h"
#include "Core/Pictorum/PictorumShapeDrawer.h"

PictorumWidget::PictorumWidget(const TString& Name) : EngineObject(Name) {
	Rotation = 0.0f; // 0 Degrees
	Parent = nullptr;
	OwningRenderer = nullptr;
	bWasClickInside = false;
	bIsBeginHovered = false;

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
	MaxBounds.x = MathLibrary::Min(MaxBounds.x, LastRenderedGeometry.GetMaximumClipPoint().x);
	MaxBounds.y = MathLibrary::Min(MaxBounds.y, LastRenderedGeometry.GetMaximumClipPoint().y);
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
		return slot;
	}
	return nullptr;
}
bool PictorumWidget::RemoveChild(PictorumWidget* Widget) {
	if (Children.Remove(Widget)) {
		Widget->RemovedFromParent(this);
		delete Widget->GetParentSlot<IWidgetSlot>();
		return true;
	}
	return false;
}
const bool PictorumWidget::CanAddChild() const {
	return true;
}
PictorumWidget* PictorumWidget::GetChildAtIndex(int32 Index) const {
	return Children[Index];
}
IWidgetSlot* PictorumWidget::CreateSlotForWidget(PictorumWidget* WidgetForSlot) const {
	return new IWidgetSlot();
}

mat4 PictorumWidget::CalculateModelMatrix(const FRenderGeometry& Geometry) const {
	vec2 relativeLocation = Geometry.GetLocation(EPictorumLocationBasis::NDC);
	vec2 relativeScale = 2.0f * Geometry.GetAllotedSpace(EPictorumScaleBasis::RELATIVE);
	float screenRotation = GetRenderRotation();

	mat4 posMatrix = glm::translate(vec3(relativeLocation, 0.0f));
	mat4 scaleMatrix = glm::scale(vec3(relativeScale, 0.0f));
	mat4 rotXMatrix = glm::rotate(0.0f, vec3(1, 0, 0));
	mat4 rotYMatrix = glm::rotate(0.0f, vec3(0, 1, 0));
	mat4 rotZMatrix = glm::rotate(screenRotation, vec3(0, 0, 1));

	mat4 combinedRotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;

	return posMatrix * scaleMatrix * combinedRotMatrix;
}
void PictorumWidget::DrawContents(const float& DeltaTime, const FRenderGeometry& Geometry) {
	// Do not render any widgets that are outside their parent's bounds.
	vec2 childMin = Geometry.GetLocation();
	vec2 childMax = childMin + Geometry.GetAllotedSpace();

	if (childMax.y < Geometry.GetMinimumClipPoint().y || childMin.y > Geometry.GetMaximumClipPoint().y) {
		return;
	}
	if (childMax.x < Geometry.GetMinimumClipPoint().x || childMin.x > Geometry.GetMaximumClipPoint().x) {
		return;
	}


	LastRenderedGeometry = Geometry;
	OnDrawStart(DeltaTime, Geometry);
	Draw(DeltaTime, Geometry);

	for (int i = 0; i < Children.Count(); i++) {
		PictorumWidget* widget = Children[i];
		FRenderGeometry childGeometry(Geometry);
		CalculateChildRenderGeometry(Geometry, childGeometry, i);
		glScissor((GLuint)childGeometry.GetMinimumClipPoint().x, (GLuint)childGeometry.GetMinimumClipPoint().y, (GLuint)childGeometry.GetMaximumClipPoint().x, (GLuint)childGeometry.GetMaximumClipPoint().y);

		widget->DrawContents(DeltaTime, childGeometry);
		OnChildDrawn(DeltaTime, Geometry);
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
void PictorumWidget::AddedToParent(PictorumWidget* ParentIn, IWidgetSlot* Slot) {
	Parent = ParentIn;
	ParentSlot = Slot;
	OnCreated();
}
void PictorumWidget::RemovedFromParent(PictorumWidget* ParentIn) {
	Parent = nullptr;
	ParentSlot = nullptr;
	OnDestroyed();
}
void PictorumWidget::AddedToViewport(PictorumRenderer* Owner) {
	OwningRenderer = Owner;
	OnCreated();
	OnAddedToViewport(Owner);
}
void PictorumWidget::RemovedFromViewport() {
	OnRemovedFromViewport();
	OwningRenderer = nullptr;
	OnDestroyed();
}

void PictorumWidget::SetVisibility(EPictorumVisibilityState NewVisibility) {
	Visibility = NewVisibility;
}
EPictorumVisibilityState PictorumWidget::GetVisibility() const {
	return Visibility;
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
PictorumWidget* PictorumWidget::GetParent() const {
	return Parent;
}
const PictorumRenderer* PictorumWidget::GetOwningRenderer() const {
	if (OwningRenderer) {
		return OwningRenderer;
	} else if (Parent) {
		return Parent->GetOwningRenderer();
	} else {
		SD_ENGINE_ERROR("Pictorum widget: {0} made a request for its owning renderer and was unable to find one up the parent chain!", GetName());
		return nullptr;
	}
}
const bool& PictorumWidget::IsHovered() const {
	return bIsBeginHovered;
}
const bool& PictorumWidget::WasClickedInside() const {
	return bWasClickInside;
}

void PictorumWidget::DrawQuad(const FRenderGeometry& Geometry, const FBoxDrawInstruction& Instruction) const {
	GetOwningRenderer()->GetShapeDrawer()->DrawQuad(Geometry, Instruction);
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
void PictorumWidget::OnChildDrawn(const float& DeltaTime, const FRenderGeometry& Geometry) {}
void PictorumWidget::OnDrawCompleted(const float& DeltaTime, const FRenderGeometry& Geometry) {}
void PictorumWidget::OnMouseEnter(const vec2& MousePosition, FUserInterfaceEvent& Event) {}
void PictorumWidget::OnMouseExit(const vec2& MousePosition, FUserInterfaceEvent& Event) {}
void PictorumWidget::OnMouseMove(const vec2& MousePosition, const vec2& MouseDelta, FUserInterfaceEvent& Event) {}
void PictorumWidget::OnMouseDown(const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& Event) {}
void PictorumWidget::OnMouseUp(const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& Event) {}