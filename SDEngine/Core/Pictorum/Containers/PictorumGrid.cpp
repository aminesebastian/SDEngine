#include "PictorumGrid.h"

PictorumGrid::PictorumGrid(const TString& Name) : PictorumWidget(Name) {
	bResizeable = true;
}
PictorumGrid::~PictorumGrid() {
	for (FGridRule* rule : Columns) {
		delete rule;
	}
	for (FGridRule* rule : Rows) {
		delete rule;
	}
	Columns.Clear();
	Rows.Clear();
}
void PictorumGrid::CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const {
	PictorumGridSlot* slot = Children[ChildIndex]->GetParentSlot<PictorumGridSlot>();

	Vector2D minExtent, maxExtent;
	GetSlotDimensions(slot->Row, slot->Column, slot->RowSpan, slot->ColumnSpan, CurrentRenderGeometry, minExtent, maxExtent);

	OutputGeometry.SetLocation(minExtent);
	OutputGeometry.SetAllotedSpace(maxExtent);
}
const int32 PictorumGrid::AddColumn(const float& ColumnWidth, bool Relative) {
	Columns.Add(new FGridRule(ColumnWidth, Relative, true));
	return Columns.LastIndex();
}
const int32 PictorumGrid::AddRow(const float& RowHeight, bool Relative) {
	Rows.Add(new FGridRule(RowHeight, Relative, true));
	return Rows.LastIndex();
}
void PictorumGrid::SetColumnWidth(const int32& Column, const float& ColumnWidth, bool Relative) {
	Columns[Column]->Value = ColumnWidth;
	Columns[Column]->bRelative = Relative;
}
void PictorumGrid::SetRowHeight(const int32& Row, const float& RowHeight, bool Relative) {
	Rows[Row]->Value = RowHeight;
	Rows[Row]->bRelative = Relative;
}
void PictorumGrid::GetSlotDimensions(const int32& Row, const int32& Column, const int32& RowSpan, const int32& ColumnSpan, const FRenderGeometry& CurrentRenderGeometry, Vector2D& MinExtent, Vector2D& MaxExtent) const {
	MinExtent    = CurrentRenderGeometry.GetLocation();
	MinExtent.y += CurrentRenderGeometry.GetAllotedSpace().y;

	for (int32 i = 0; i <= Row; i++) {
		MinExtent.y -= Rows[i]->GetValue(CurrentRenderGeometry).y;
	}
	for (int32 i = 0; i < Column; i++) {
		MinExtent.x += Columns[i]->GetValue(CurrentRenderGeometry).x;
	}

	MaxExtent = CurrentRenderGeometry.GetAllotedSpace();

	for (int32 i = Row; i < MathLibrary::Min(Row + RowSpan, Rows.Count()); i++) {
		MaxExtent.y -= Rows[i]->GetValue(CurrentRenderGeometry).y;
	}
	for (int32 i = Column; i < MathLibrary::Min(Column + ColumnSpan, Columns.Count()); i++) {
		MaxExtent.x -= Columns[i]->GetValue(CurrentRenderGeometry).x;
	}

}
Vector2D PictorumGrid::CalculateTotalDimensions(const FRenderGeometry& Geometry) const {
	float width = 0.0f;
	float height = 0.0f;

	for (FGridRule* rule : Columns) {
		if (rule->bRelative) {
			width += rule->Value * Geometry.GetAllotedSpace().x;
		} else {
			width += rule->Value;
		}
	}
	for (FGridRule* rule : Rows) {
		if (rule->bRelative) {
			height += rule->Value * Geometry.GetAllotedSpace().y;
		} else {
			height += rule->Value;
		}
	}

	return Vector2D(width, height);
}
PictorumGridSlot* PictorumGrid::AddChild(PictorumWidget* Widget) {
	return Cast<PictorumGridSlot>(AddChildInternal(Widget));
}
PictorumGridSlot* PictorumGrid::CreateSlotForWidget(PictorumWidget* WidgetForSlot) const {
	return new PictorumGridSlot();
}