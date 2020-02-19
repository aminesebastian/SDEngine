#pragma once
#include "Core/Assets/AssetManager.h"
#include "Core/DataStructures/Array.h"
#include "Core/Engine/Delegates/Event.h"
#include "Core/Engine/Engine.h"
#include "Core/Input/InputSubsystem.h"
#include "Core/Input/InputUtilities.h"
#include "Core/Objects/EngineObject.h"
#include "Core/Pictorum/IWidgetSlot.h"
#include "Core/Pictorum/PictorumDataTypes.h"
#include "Core/Utilities/EngineFunctionLibrary.h"
#include "Core/Utilities/Logger.h"
#include "Core/Utilities/Math/Transform.h"
#include "Core/Utilities/Math/MathLibrary.h"
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

	/**
	 * This method is raised either when this widget is added to the viewport (if this is a top
	 * level widget) or when it is added to a parent After this point, it is safe to reference all
	 * widget fields (parent, owning window, etc).
	 */
	virtual void OnCreated();
	/**
	 * This method is raised either when this widget is removed from the viewport (if this is a top
	 * level widget) or when it is removed from its parent. After this point, it is no longer safe
	 * to reference any widget fields (parent, owning window, etc).
	 */
	virtual void OnDestroyed();
	virtual void Tick(float DeltaTime, const FRenderGeometry& Geometry);
	virtual void Draw(float DeltaTime, const FRenderGeometry& Geometry);
	virtual vec2 GetDesiredDrawSpace(const FRenderGeometry& Geometry) const;
	virtual void CalculateBounds(vec2 RenderTargetResolution, vec2& MinBounds, vec2& MaxBounds) const;
	virtual void CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const;

	void SetVisibility(EPictorumVisibilityState NewVisibility);
	EPictorumVisibilityState GetVisibility() const;
	vec2 GetPivotOffset() const;

	void GetAllChildren(SArray<PictorumWidget*>& ChildrenOut, bool bIncludeAllDescendents = false) const;
	PictorumWidget* GetParent() const;

	virtual IWidgetSlot* AddChild(PictorumWidget* Widget);
	virtual bool RemoveChild(PictorumWidget* Widget);
	virtual const bool CanAddChild() const;
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

	virtual void OnMouseEnter(vec2 MousePosition, FUserInterfaceEvent& EventIn);
	virtual void OnMouseExit(vec2 MousePosition, FUserInterfaceEvent& EventIn);
	virtual void OnMouseMove(vec2 MousePosition, vec2 MouseDelta, FUserInterfaceEvent& EventIn);
	virtual void OnMouseDown(vec2 MousePosition, EMouseButton Button, FUserInterfaceEvent& EventIn);
	virtual void OnMouseUp(vec2 MousePosition, EMouseButton Button, FUserInterfaceEvent& EventIn);

	virtual TString GetDetailsPanelName() override;
	virtual bool PopulateDetailsPanel() override;

	const float GetRenderRotation() const;
	const float GetRotation() const;
	const float GetParentRotation() const;
	const PictorumRenderer* GetOwningRenderer() const;

	Event<void(PictorumWidget*, const vec2&, FUserInterfaceEvent&)> OnMouseDownDelegate;
	Event<void(PictorumWidget*, const vec2&, FUserInterfaceEvent&)> OnMouseUpDelegate;
	Event<void(PictorumWidget*, const vec2&, FUserInterfaceEvent&)> OnHoveredDelegate;
	Event<void(PictorumWidget*, const vec2&, FUserInterfaceEvent&)> OnUnhoveredDelegate;
	Event<void(PictorumWidget*, const vec2&, const vec2&, FUserInterfaceEvent&)> OnMouseMoveDelegate;

protected:
	/** All the children of this widget. To get the slot for a particular child, you must find that child and then query it for the slot. */
	SArray<PictorumWidget*> Children;
	/** The widget this parent exists in. If null, this widget exists at the room of the widget tree. */
	PictorumWidget* Parent;
	/** The slot this widget occupies in its parent. The lifecyle of this object is managed by the parent. */
	IWidgetSlot* ParentSlot;
	/** The owning renderer viewport of this widget. */
	PictorumRenderer* OwningRenderer;

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
	* This method rarely should be overwritten and should only be called by the renderer.
	* This first calls the Draw method of the current widget, then calls DrawContents() on each child.
	* This processes all children from the root widget.
	*/
	virtual void DrawContents(const float& DeltaTime, const FRenderGeometry& Geometry);
	virtual void TickContents(const float& DeltaTime, const FRenderGeometry& Geometry);
	virtual void OnDrawStart(const float& DeltaTime, const FRenderGeometry& Geometry);
	virtual void OnChildDrawn(const float& DeltaTime, const FRenderGeometry& Geometry);
	virtual void OnDrawCompleted(const float& DeltaTime, const FRenderGeometry& Geometry);

	virtual IWidgetSlot* AddChildInternal(PictorumWidget* Widget);
	virtual IWidgetSlot* CreateSlotForWidget(PictorumWidget* WidgetForSlot) const;
	mat4 CalculateModelMatrix(const FRenderGeometry& Geometry) const;
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
	/**
	 *
	 * This method is raised when a widget is added to a renderer at the root level.
	 *
	 * @param [in,out]	{PictorumRenderer*}	Owner	The owning renderer.
	 */
	void OnAddedToViewport(PictorumRenderer* Owner);

	/** This method is raised when this widget is removed from the renderer at the root level. */
	void OnRemovedFromViewport();
private:
	friend class PictorumRenderer;
	bool bWasMouseDownInside;
	bool bDidMouseEnter;
};
