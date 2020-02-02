#pragma once
#include "Core/DataStructures/Array.h"
#include "Core/Input/InputUtilities.h"
#include "Core/Pictorum/PictorumDataTypes.h"
#include <GLEW/glew.h>
#include <GLM\glm.hpp>
#include <GLM\gtx\transform.hpp>
#include "Utilities/Transform.h"
#include "UserInterface/DetailsPanelProvider.h"
#include "Engine/EngineObject.h"

class Shader;
class DragFloat;


class PictorumWidget : public EngineObject, public IDetailsPanelProvider {
public:
	PictorumWidget(TString Name);
	~PictorumWidget();

	virtual void Tick(float DeltaTime, FRenderGeometry Geometry);
	virtual void Draw(float DeltaTime, FRenderGeometry Geometry);
	virtual void CalculateBounds(vec2 RenderTargetResolution, vec2& MinBounds, vec2& MaxBounds);

	void SetHorizontalAlignment(EHorizontalAlignment Alignment);
	EHorizontalAlignment GetHorizontalAlignment() const;
	void SetVerticalAlignment(EVerticalAlignment Alignment);
	EVerticalAlignment GetVerticalAlignment() const;
	void SetFillState(EFillState State);
	EFillState GetFillState() const;

	void SetMargins(FMargins NewMargins);
	void SetMargins(float Top, float Right, float Left, float Bottom);
	FMargins GetMargins() const;
	void SetPadding(FPadding NewPadding);
	void SetPadding(float Top, float Right, float Left, float Bottom);
	FPadding GetPadding() const;
	void SetVisibility(EPictorumVisibilityState NewVisibility);
	EPictorumVisibilityState GetVisibility() const;

	void GetAllChildren(SArray<PictorumWidget*>& ChildrenOut) const;
	PictorumWidget* GetParent() const;

	virtual bool AddChild(PictorumWidget* Widget);
	virtual bool RemoveChild(PictorumWidget* Widget);

	virtual void OnAddedToParent(PictorumWidget* ParentIn);
	virtual void OnRemovedFromParent(PictorumWidget* ParentIn);

	virtual void OnMouseEnter(vec2 MousePosition, FUserInterfaceEvent& Event);
	virtual void OnMouseExit(vec2 MousePosition, FUserInterfaceEvent& Event);
	virtual void OnMouseMove(vec2 MousePosition, vec2 MouseDelta, FUserInterfaceEvent& Event);
	virtual void OnMouseDown(vec2 MousePosition, EMouseButton Button, FUserInterfaceEvent& Event);
	virtual void OnMouseUp(vec2 MousePosition, EMouseButton Button, FUserInterfaceEvent& Event);

	virtual TString GetDetailsPanelName() override;
	virtual bool PopulateDetailsPanel() override;

protected:
	/**
	 * This method rarely should be overwritten and should only be called by the renderer.
	 * This first calls the Draw method of the current widget, then calls DrawContents() on each child.
	 * This processes all children from the root widget.
	 */
	virtual void DrawContents(float DeltaTime, FRenderGeometry Geometry);
	vec2 GetParentLocation() const;
	vec2 GetParentScale() const;
	float GetParentRotation() const;

	vec2 GetAbsoluteLayoutLocation() const;
	vec2 GetAbsoluteLayoutScale() const;
	float GetAbsoluteLayoutRotation() const;

	mat4 CalculateModelMatrix(const vec2& ScreenDimensions) const;

	SArray<PictorumWidget*> Children;
	PictorumWidget* Parent;
	SArray<DragFloat*> DetailsPanelWidgets;

	vec2 Location;
	vec2 Scale;
	float Rotation;

	FMargins Margins;
	FPadding Padding;
	EPictorumVisibilityState Visibility;

	EVerticalAlignment VerticalAlignment;
	EHorizontalAlignment HorizontalAlignment;
	EFillState FillState;

private:
	friend class PictorumRenderer;
};
