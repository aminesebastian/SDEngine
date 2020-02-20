#pragma once
#include "Core/Pictorum/PictorumWidget.h"
#include "Core/Pictorum/Slots/PictorumGridSlot.h"

struct FGridRule {
	float Value;
	bool bIsRow;
	bool bRelative;
	bool bResizeable;

	FGridRule(const float& Value, const bool& bRelative, const bool& bResizeable, const bool& bIsRow) : Value(Value), bRelative(bRelative), bResizeable(bResizeable), bIsRow(bIsRow) {}
	const float GetValue(const FRenderGeometry& Geometry) {
		if (bRelative) {
			return bIsRow ? Geometry.GetAllotedSpace().y * Value : Geometry.GetAllotedSpace().x * Value;
		} else {
			return Value;
		}
	}
	void AddRelativeValue(const float& ValueIn) {
		Value = MathLibrary::Max(ValueIn, 1.0f);
	}
};

class PictorumGrid : public PictorumWidget {
public:
	PictorumGrid(const TString& Name);
	virtual ~PictorumGrid();

	virtual void CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const override;

	const int32 AddColumn(const float& ColumnWidth, bool Relative);
	const int32 AddRow(const float& RowHeight, bool Relative);
	void SetColumnWidth(const int32& Column, const float& ColumnWidth, bool Relative = true);
	void SetRowHeight(const int32& Row, const float& RowHeight, bool Relative = true);
 
	virtual PictorumGridSlot* AddChild(PictorumWidget* Widget) override;
	virtual PictorumGridSlot* CreateSlotForWidget(PictorumWidget* WidgetForSlot) const override;

	void Tick(float DeltaTime, const FRenderGeometry& Geometry) override;
protected:
	SArray<FGridRule*> Columns;
	SArray<FGridRule*> Rows;
	bool bResizeable;
	int32 ResizeHandleDistance;
	int32 HoveredColumnIndex;
	int32 HoveredRowIndex;
	Vector2D DistanceToNearestEntry;
	Vector2D CellRelativePosition;

	void GetSlotDimensions(const int32& Row, const int32& Column, const int32& RowSpan, const int32& ColumnSpan, const FRenderGeometry& CurrentRenderGeometry, Vector2D& Location, Vector2D& Space) const;
	Vector2D CalculateTotalDimensions(const FRenderGeometry& Geometry) const;

	virtual void OnMouseEnter(const vec2& MousePosition, FUserInterfaceEvent& EventIn) override;
	virtual void OnMouseExit(const vec2& MousePosition, FUserInterfaceEvent& EventIn) override;
	virtual void OnMouseMove(const vec2& MousePosition, const vec2& MouseDelta, FUserInterfaceEvent& EventIn) override;
	virtual void OnMouseDown(const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& EventIn) override;
	virtual void OnMouseUp(const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& EventIn) override;

private:
	bool GetTargetResizeColumns(int32& GrowColumnIndex, int32& ShrinkColumnIndex);
	bool GetTargetResizeRows(int32& GrowRowIndex, int32& ShrinkRowIndex);
	void CacheHoveredCellValues(const vec2& MousePosition);
};


