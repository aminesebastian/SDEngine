#pragma once
#include "Core/Pictorum/PictorumWidget.h"
#include "Core/Pictorum/Slots/PictorumGridSlot.h"

struct FGridRule {
	FGridRule(const float& Value, const float& MinSize, const bool& bRelative, const bool& bResizeable, const bool& bIsRow) : Value(Value), MinSize(MinSize), bRelative(bRelative), bResizeable(bResizeable), bIsRow(bIsRow) {

	}
	const float GetValue(const FRenderGeometry& Geometry) {
		if (bRelative) {
			return bIsRow ? Geometry.GetAllotedSpace().y * Value : Geometry.GetAllotedSpace().x * Value;
		} else {
			return Value;
		}
	}
	float AddValue(const float& ValueIn, const FRenderGeometry& Geometry, bool bValueInRelative = true) {
		float initialValue = Value;
		float adjustedMinSize = bIsRow ? MinSize / Geometry.GetRenderResolution().y : MinSize / Geometry.GetRenderResolution().x;
		Value = MathLibrary::Clamp(Value + ValueIn, adjustedMinSize, 1.0f);
		return initialValue - Value;
	}
private:
	friend class PictorumGrid;
	float MinSize;
	float Value;
	bool bIsRow;
	bool bRelative;
	bool bResizeable;
};

class PictorumGrid : public PictorumWidget {
public:

	/** The minimum size of the grid space in absolute pixels. */
	float MinimumGridSpaceSize;

	PictorumGrid(const TString& Name);
	virtual ~PictorumGrid();

	virtual void Draw(float DeltaTime, const FRenderGeometry& Geometry) override;
	virtual void CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const override;
	virtual PictorumGridSlot* AddChild(PictorumWidget* Widget) override;

	const int32 AddColumn(const float& ColumnWidth, bool Relative = true);
	const int32 AddRow(const float& RowHeight, bool Relative = true);
	void SetColumnWidth(const int32& Column, const float& ColumnWidth, bool Relative = true);
	void SetRowHeight(const int32& Row, const float& RowHeight, bool Relative = true);

protected:
	SArray<FGridRule*> Columns;
	SArray<FGridRule*> Rows;
	bool bResizeable;
	int32 ResizeHandleDistance;
	int32 HoveredColumnIndex;
	int32 HoveredRowIndex;
	Vector2D DistanceToNearestEntry;
	Vector2D CellRelativePosition;


	/************/
	/* RESIZING */
	/************/
	bool bCanResize;
	bool bWasMouseClickedDuringResize;
	FGridRule* ShrinkTarget;
	FGridRule* GrowTarget;
	int32 ShrinkTargetIndex;
	int32 GrowTargetIndex;

	virtual PictorumGridSlot* CreateSlotForWidget(PictorumWidget* WidgetForSlot) const override;
	void GetSlotDimensions(const int32& Row, const int32& Column, const int32& RowSpan, const int32& ColumnSpan, const FRenderGeometry& CurrentRenderGeometry, Vector2D& Location, Vector2D& Space) const;
	Vector2D CalculateTotalDimensions(const FRenderGeometry& Geometry) const;
	PictorumWidget* GetWidgetInSlot(const int32& Row, const int32& Column);
	PictorumGridSlot* GetSlotForWidgetInSlot(const int32& Row, const int32& Column);

	virtual const EMouseCursorStyle GetMouseCursor(const vec2& MousePosition, bool& Override) override;
	virtual void OnMouseExit(const vec2& MousePosition, FUserInterfaceEvent& EventIn) override;
	virtual void OnMouseMove(const vec2& MousePosition, const vec2& MouseDelta, FUserInterfaceEvent& EventIn) override;
	virtual void OnMouseDown(const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& EventIn) override;
	virtual void OnMouseUp(const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& EventIn) override;
private:
	bool UpdateResizeTargets();
	void CacheMouseMoveValues(const vec2& MousePosition);
	/**
	 * Gets the minimum height for a row in relative units.
	 *
	 * @param 	RowIndex	Zero-based index of the row.
	 *
	 * @returns	The row minimum height.
	 */
	const float GetRowMinSize(const int32& RowIndex);

	/**
	 * Gets the minimum width for a column in relative units.
	 *
	 * @param 	ColumnIndex	Zero-based index of the column.
	 *
	 * @returns	The column minimum width.
	 */
	const float GetColumnMinSize(const int32& ColumnIndex);
};


