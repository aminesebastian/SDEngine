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
#include "Core/Pictorum/PictorumRenderer.h"
#include "Core/Utilities/EngineFunctionLibrary.h"
#include "Core/Utilities/Logger.h"
#include "Core/Utilities/Math/MathLibrary.h"
#include "Core/Utilities/Math/Transform.h"
#include <GLEW/glew.h>
#include <GLM\glm.hpp>
#include <GLM\gtx\transform.hpp>

class Shader;
class DragFloat;

class PictorumWidget : public EngineObject {
public:
	PictorumWidget(const TString& Name);
	virtual ~PictorumWidget();

	virtual void Tick(float DeltaTime, const FRenderGeometry& Geometry);
	virtual void Draw(float DeltaTime, const FRenderGeometry& Geometry);
	virtual vec2 GetDesiredDrawSpace(const FRenderGeometry& Geometry) const;
	virtual void CalculateBounds(vec2 RenderTargetResolution, vec2& MinBounds, vec2& MaxBounds) const;
	virtual void CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const;

	void SetVisibility(EPictorumVisibilityState NewVisibility);
	EPictorumVisibilityState GetVisibility() const;
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

	const float GetRenderRotation() const;
	const float GetRotation() const;
	const float GetParentRotation() const;

	Event<void(PictorumWidget*, const vec2&, const EMouseButton&, FUserInterfaceEvent&)> OnMouseDownDelegate;
	Event<void(PictorumWidget*, const vec2&, const EMouseButton&, FUserInterfaceEvent&)> OnMouseUpDelegate;
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
	virtual mat4 CalculateModelMatrix(const FRenderGeometry& Geometry) const;

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
	virtual void OnAddedToViewport(PictorumRenderer* Owner);
	/** This method is raised when this widget is removed from the renderer at the root level. */
	virtual void OnRemovedFromViewport();
	virtual void OnMouseEnter(const vec2& MousePosition, FUserInterfaceEvent& EventIn);
	virtual void OnMouseExit(const vec2& MousePosition, FUserInterfaceEvent& EventIn);
	virtual void OnMouseMove(const vec2& MousePosition, const vec2& MouseDelta, FUserInterfaceEvent& EventIn);
	virtual void OnMouseDown(const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& EventIn);
	virtual void OnMouseUp(const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& EventIn);

	void DrawQuad(const FRenderGeometry& Geometry, const FBoxDrawInstruction& Instruction) const;
	void DrawImage(const FRenderGeometry& Geometry, const FImageDrawInstruction& Instruction) const;

	const bool& IsHovered() const;
	const bool& WasClickedInside() const;
	const PictorumRenderer* GetOwningRenderer() const;
private:
	friend class PictorumRenderer;
	bool bWasClickInside;
	bool bIsBeginHovered;

	virtual void AddedToParent(PictorumWidget* ParentIn, IWidgetSlot* Slot);
	virtual void RemovedFromParent(PictorumWidget* ParentIn);
	virtual void AddedToViewport(PictorumRenderer* Owner);
	virtual void RemovedFromViewport();
	virtual void MouseEnter(const vec2& MousePosition, FUserInterfaceEvent& EventIn);
	virtual void MouseExit(const vec2& MousePosition, FUserInterfaceEvent& EventIn);
	virtual void MouseMove(const vec2& MousePosition, const vec2& MouseDelta, FUserInterfaceEvent& EventIn);
	virtual void MouseDown(const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& EventIn);
	virtual void MouseUp(const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& EventIn);
};
