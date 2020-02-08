#pragma once
#include "Core/Assets/AssetManager.h"
#include "Core/DataStructures/Array.h"
#include "Core/Engine/Engine.h"
#include "Core/Input/InputUtilities.h"
#include "Core/Objects/EngineObject.h"
#include "Core/Pictorum/IWidgetSlot.h"
#include "Core/Pictorum/PictorumDataTypes.h"
#include "Core/Utilities/EngineFunctionLibrary.h"
#include "Core/Utilities/Math/Transform.h"
#include "UserInterface/DetailsPanelProvider.h"
#include <GLEW/glew.h>
#include <GLM\glm.hpp>
#include <GLM\gtx\transform.hpp>

class Shader;
class DragFloat;

class PictorumWidget : public EngineObject, public IDetailsPanelProvider {
public:
	PictorumWidget(const TString& Name);
	virtual ~PictorumWidget();

	virtual void OnCreated(FRenderGeometry Geometry);
	virtual void OnDestroyed(FRenderGeometry Geometry);
	virtual void Tick(float DeltaTime, const FRenderGeometry& Geometry);
	virtual void Draw(float DeltaTime, const FRenderGeometry& Geometry);
	virtual vec2 GetDesiredDrawSpace(const FRenderGeometry& Geometry) const;
	virtual void CalculateBounds(vec2 RenderTargetResolution, vec2& MinBounds, vec2& MaxBounds) const;
	virtual void CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const;

	void SetVisibility(EPictorumVisibilityState NewVisibility);
	EPictorumVisibilityState GetVisibility() const;
	vec2 GetPivotOffset() const;

	void GetAllChildren(SArray<PictorumWidget*>& ChildrenOut) const;
	PictorumWidget* GetParent() const;

	virtual IWidgetSlot* AddChild(PictorumWidget* Widget);
	virtual bool RemoveChild(PictorumWidget* Widget);
	virtual bool CanAddChild() const;
	PictorumWidget* GetChildAtIndex(int32 Slot) const;
	template<typename T>
	T* GetChildSlotAtIndex(const int32& Index) const {
		static_assert(std::is_base_of<IWidgetSlot, T>::value, "T must derive from IWidgetSlot!");
		return Children[Index]->GetParentSlot<T>();
	}
	template<typename T>
	T* GetParentSlot() const {
		static_assert(std::is_base_of<IWidgetSlot, T>::value, "T must derive from IWidgetSlot!");
		return Cast<T>(ParentSlot);
	}

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
	virtual void DrawContents(float DeltaTime, const FRenderGeometry& Geometry);
	virtual IWidgetSlot* AddChildInternal(PictorumWidget* Widget);
	virtual IWidgetSlot* CreateSlotForWidget(PictorumWidget* WidgetForSlot) const;
	mat4 CalculateModelMatrix(const FRenderGeometry& Geometry) const;

	/** All the children of this widget. To get the slot for a particular child, you must find that child and then query it for the slot. */
	SArray<PictorumWidget*> Children;
	/** The widget this parent exists in. If null, this widget exists at the room of the widget tree. */
	PictorumWidget* Parent;
	/** The slot this widget occupies in its parent. The lifecyle of this object is managed by the parent. */
	IWidgetSlot* ParentSlot;
	SArray<DragFloat*> DetailsPanelWidgets;

	/** Rotation of the widget [0-360n] */
	float Rotation;
	/** This value represents the pivot point of the widget [0, 1] */
	vec2 PivotOffset;
	/** The visibility state of the widget */
	EPictorumVisibilityState Visibility;

	/** Gets the latest cached render geometry. */
	FRenderGeometry LastRenderedGeometry;

	/**
	 * This method is raised when the widget is added to a parent container.
	 *
	 * @param [in,out]	{PictorumWidget*}	ParentIn	If non-null, the parent in.
	 */
	virtual void OnAddedToParent(PictorumWidget* ParentIn, IWidgetSlot* Slot);

	/**
	 * This method is raised when the widget is removed from it's parent container.
	 *
	 * @param [in,out]	{PictorumWidget*}	ParentIn	If non-null, the parent in.
	 */
	virtual void OnRemovedFromParent(PictorumWidget* ParentIn);
private:
	friend class PictorumRenderer;

};
