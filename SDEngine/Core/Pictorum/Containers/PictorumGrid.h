#pragma once
#include "Core/Pictorum/PictorumWidget.h"
#include "Core/Pictorum/Slots/PictorumGridSlot.h"

struct FGridRule {
	float Value;
	bool bRelative;
	bool bResizeable;

	FGridRule(const float& Value, const bool& bRelative, const bool& bResizeable) : Value(Value), bRelative(bRelative), bResizeable(bResizeable) {}
	const Vector2D GetValue(const FRenderGeometry& Geometry) {
		if (bRelative) {
			return Geometry.GetAllotedSpace() * Value;
		} else {
			return Vector2D(Value, Value);
		}
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

protected:
	SArray<FGridRule*> Columns;
	SArray<FGridRule*> Rows;
	bool bResizeable;

	void GetSlotDimensions(const int32& Row, const int32& Column, const int32& RowSpan, const int32& ColumnSpan, const FRenderGeometry& CurrentRenderGeometry, Vector2D& MinExtent, Vector2D& MaxExtent) const;
	Vector2D CalculateTotalDimensions(const FRenderGeometry& Geometry) const;
};


