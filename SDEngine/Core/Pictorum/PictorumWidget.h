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

	virtual void OnCreated(FRenderGeometry Geometry);
	virtual void OnDestroyed(FRenderGeometry Geometry);
	virtual void Tick(float DeltaTime, const FRenderGeometry& Geometry);
	virtual void Draw(float DeltaTime, const FRenderGeometry& Geometry);
	virtual vec2 GetDesiredDrawSpace(const FRenderGeometry& Geometry);
	virtual void CalculateBounds(vec2 RenderTargetResolution, vec2& MinBounds, vec2& MaxBounds);
	virtual void CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex);

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
	vec2 GetPivotOffset() const;

	void GetAllChildren(SArray<PictorumWidget*>& ChildrenOut) const;
	PictorumWidget* GetParent() const;

	virtual bool AddChild(PictorumWidget* Widget);
	virtual bool RemoveChild(PictorumWidget* Widget);

	virtual void OnMouseEnter(vec2 MousePosition, FUserInterfaceEvent& Event);
	virtual void OnMouseExit(vec2 MousePosition, FUserInterfaceEvent& Event);
	virtual void OnMouseMove(vec2 MousePosition, vec2 MouseDelta, FUserInterfaceEvent& Event);
	virtual void OnMouseDown(vec2 MousePosition, EMouseButton Button, FUserInterfaceEvent& Event);
	virtual void OnMouseUp(vec2 MousePosition, EMouseButton Button, FUserInterfaceEvent& Event);

	virtual TString GetDetailsPanelName() override;
	virtual bool PopulateDetailsPanel() override;

	float GetRenderRotation() const;
	float GetRotation() const;
	float GetParentRotation() const;

protected:
	/**
	 * This method rarely should be overwritten and should only be called by the renderer.
	 * This first calls the Draw method of the current widget, then calls DrawContents() on each child.
	 * This processes all children from the root widget.
	 */
	virtual void DrawContents(float DeltaTime, FRenderGeometry Geometry);

	mat4 CalculateModelMatrix(const FRenderGeometry& Geometry) const;

	SArray<PictorumWidget*> Children;
	PictorumWidget* Parent;
	SArray<DragFloat*> DetailsPanelWidgets;

	/** Rotation of the widget [0-360n] */
	float Rotation;
	/** This value represents the pivot point of the widget [0, 1] */
	vec2 PivotOffset;
	/** The margins of the widget. */
	FMargins Margins;
	/** The internal padding of the widget*/
	FPadding Padding;
	/** The visibility state of the widget */
	EPictorumVisibilityState Visibility;

	/** Gets the latest cached render geometry. */
	FRenderGeometry LastRenderedGeometry;

	/** The vertical alignment in it's parent container (if applicable). */
	EVerticalAlignment VerticalAlignment;
	/** The horizontal alignment in it's parent container (if applicable). */
	EHorizontalAlignment HorizontalAlignment;
	/** The fill state in it's parent container (if applicable). */
	EFillState FillState;

	/**
	 * This method is raised when the widget is added to a parent container.
	 *
	 * @param [in,out]	{PictorumWidget*}	ParentIn	If non-null, the parent in.
	 */
	virtual void OnAddedToParent(PictorumWidget* ParentIn);

	/**
	 * This method is raised when the widget is removed from it's parent container.
	 *
	 * @param [in,out]	{PictorumWidget*}	ParentIn	If non-null, the parent in.
	 */
	virtual void OnRemovedFromParent(PictorumWidget* ParentIn);
private:
	friend class PictorumRenderer;

};
