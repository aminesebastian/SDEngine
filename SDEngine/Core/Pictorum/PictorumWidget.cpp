#include "PictorumWidget.h"
#include "Core/Engine/EngineStatics.h"
#include "Core/Engine/Engine.h"
#include "Core/Rendering/RenderViewport.h"
#include "Core/Rendering/DefferedCompositor.h"
#include "Core/Objects/CoreTypes/Shader.h"
#include "Core/Utilities/Logger.h"

PictorumWidget::PictorumWidget(const TString& Name) : EngineObject(Name) {
	Rotation            = 0.0f; // 0 Degrees
	Parent              = nullptr;
	OwningRenderer      = nullptr;
	PivotOffset		    = vec2(0.0f, 0.0f);
	bWasMouseDownInside = false;
	bDidMouseEnter      = false;

	SetVisibility(EPictorumVisibilityState::VISIBLE);
}
PictorumWidget::~PictorumWidget() {

}
void PictorumWidget::OnCreated() {

}
void PictorumWidget::OnDestroyed() {

}
void PictorumWidget::Tick(float DeltaTime, const FRenderGeometry& Geometry) {

}
void PictorumWidget::Draw(float DeltaTime, const FRenderGeometry& Geometry) {

}
vec2 PictorumWidget::GetDesiredDrawSpace(const FRenderGeometry& Geometry) const {
	return Geometry.GetRenderResolution();
}
void PictorumWidget::CalculateBounds(vec2 RenderTargetResolution, vec2& MinBounds, vec2& MaxBounds) const {
	vec2 lastLocation = LastRenderedGeometry.GetLocation(EPictorumLocationBasis::ABSOLUTE);
	vec2 lastScale = LastRenderedGeometry.GetAllotedSpace(EPictorumScaleBasis::ABSOLUTE);
	MinBounds = lastLocation;
	MaxBounds = lastLocation + lastScale;
}
void PictorumWidget::CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const {
	OutputGeometry = CurrentRenderGeometry;
}

IWidgetSlot* PictorumWidget::AddChild(PictorumWidget* Widget) {
	return AddChildInternal(Widget);
}
IWidgetSlot* PictorumWidget::AddChildInternal(PictorumWidget* Widget) {
	if (!CanAddChild()) {
		return nullptr;
	}
	if (Children.AddUnique(Widget)) {
		IWidgetSlot* slot = CreateSlotForWidget(Widget);
		Widget->OnAddedToParent(this, slot);
		return slot;
	}
	return nullptr;
}
bool PictorumWidget::RemoveChild(PictorumWidget* Widget) {
	if (Children.Remove(Widget)) {
		Widget->OnRemovedFromParent(this);
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

void PictorumWidget::OnAddedToParent(PictorumWidget* ParentIn, IWidgetSlot* Slot) {
	Parent = ParentIn;
	ParentSlot = Slot;
	OnCreated();
}
void PictorumWidget::OnRemovedFromParent(PictorumWidget* ParentIn) {
	Parent = nullptr;
	ParentSlot = nullptr;
	OnDestroyed();
}

mat4 PictorumWidget::CalculateModelMatrix(const FRenderGeometry& Geometry) const {
	vec2 relativeLocation = Geometry.GetLocation(EPictorumLocationBasis::NDC);
	vec2 relativeScale    = 2.0f * Geometry.GetAllotedSpace(EPictorumScaleBasis::RELATIVE);
	float screenRotation  = GetRenderRotation();

	mat4 posMatrix   = glm::translate(vec3(relativeLocation, 0.0f));
	mat4 scaleMatrix = glm::scale(vec3(relativeScale, 0.0f));
	mat4 rotXMatrix  = glm::rotate(0.0f, vec3(1, 0, 0));
	mat4 rotYMatrix  = glm::rotate(0.0f, vec3(0, 1, 0));
	mat4 rotZMatrix  = glm::rotate(screenRotation, vec3(0, 0, 1));

	mat4 combinedRotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;

	return posMatrix * scaleMatrix * combinedRotMatrix;
}
void PictorumWidget::DrawContents(const float& DeltaTime, const FRenderGeometry& Geometry) {
	LastRenderedGeometry = Geometry;

	Draw(DeltaTime, Geometry);

	for (int i = 0; i < Children.Count(); i++) {
		PictorumWidget* widget = Children[i];
		FRenderGeometry childGeometry(Geometry);
		CalculateChildRenderGeometry(Geometry, childGeometry, i);
		widget->DrawContents(DeltaTime, childGeometry);
	}
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
void PictorumWidget::OnMouseEnter(vec2 MousePosition, FUserInterfaceEvent& Event) {
	OnHoveredDelegate.Broadcast(MousePosition, Event);
	bDidMouseEnter = true;
}
void PictorumWidget::OnMouseExit(vec2 MousePosition, FUserInterfaceEvent& Event) {
	if (bDidMouseEnter) {
		OnUnhoveredDelegate.Broadcast(MousePosition, Event);
		bDidMouseEnter = false;
	}
}
void PictorumWidget::OnMouseMove(vec2 MousePosition, vec2 MouseDelta, FUserInterfaceEvent& Event) {
	OnMouseMoveDelegate.Broadcast(MousePosition, MouseDelta, Event);
}
void PictorumWidget::OnMouseDown(vec2 MousePosition, EMouseButton Button, FUserInterfaceEvent& Event) {
	OnMouseDownDelegate.Broadcast(MousePosition, Event);
	bWasMouseDownInside = true;
}
void PictorumWidget::OnMouseUp(vec2 MousePosition, EMouseButton Button, FUserInterfaceEvent& Event) {
	if (bWasMouseDownInside) {
		OnMouseUpDelegate.Broadcast(MousePosition, Event);
		bWasMouseDownInside = false;
	}
}

void PictorumWidget::GetAllChildren(SArray<PictorumWidget*>& ChildrenOut) const {
	ChildrenOut.AddAll(Children);
	for (PictorumWidget* widget : Children) {
		widget->GetAllChildren(ChildrenOut);
	}
}
PictorumWidget* PictorumWidget::GetParent() const {
	return Parent;
}

void PictorumWidget::SetVisibility(EPictorumVisibilityState NewVisibility) {
	Visibility = NewVisibility;
}
EPictorumVisibilityState PictorumWidget::GetVisibility() const {
	return Visibility;
}
vec2 PictorumWidget::GetPivotOffset() const {
	return PivotOffset;
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
const PictorumRenderer* PictorumWidget::GetOwningRenderer() const {
	if (OwningRenderer) {
		return OwningRenderer;
	}else if (Parent) {
		return Parent->GetOwningRenderer();
	} else {
		SD_ENGINE_ERROR("Pictorum widget: {0} made a request for its owning renderer and was unable to find one up the parent chain!", GetName());
		return nullptr;
	}
}
void PictorumWidget::OnAddedToViewport(PictorumRenderer* Owner) {
	OwningRenderer = Owner;
	OnCreated();
}
void PictorumWidget::OnRemovedFromViewport() {
	OwningRenderer = nullptr;
	OnDestroyed();
}
TString PictorumWidget::GetDetailsPanelName() {
	return GetName();
}
bool PictorumWidget::PopulateDetailsPanel() {
	return true;
}