#include "PictorumGrid.h"
#include "Core/Pictorum/EngineUI/FrameStatisticsWidget.h"

PictorumGrid::PictorumGrid(const TString& Name) : PictorumWidget(Name) {
	bResizeable = true;
	HoveredColumnIndex = 0;
	HoveredRowIndex = 0;
	DistanceToNearestEntry.x = 0.0f;
	DistanceToNearestEntry.y = 0.0f;
	CellRelativePosition.x = 0.0f;
	CellRelativePosition.y = 0.0f;

	ResizeHandleDistance = 5;

	SetVisibility(EPictorumVisibilityState::VISIBLE);
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

	Vector2D location, space;
	GetSlotDimensions(slot->Row, slot->Column, slot->RowSpan, slot->ColumnSpan, CurrentRenderGeometry, location, space);

	OutputGeometry.SetLocation(location);
	OutputGeometry.SetAllotedSpace(space);
}
const int32 PictorumGrid::AddColumn(const float& ColumnWidth, bool Relative) {
	Columns.Add(new FGridRule(ColumnWidth, Relative, true, false));
	return Columns.LastIndex();
}
const int32 PictorumGrid::AddRow(const float& RowHeight, bool Relative) {
	Rows.Add(new FGridRule(RowHeight, Relative, true, true));
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

void PictorumGrid::GetSlotDimensions(const int32& Row, const int32& Column, const int32& RowSpan, const int32& ColumnSpan, const FRenderGeometry& CurrentRenderGeometry, Vector2D& Location, Vector2D& Space) const {
	Location    = CurrentRenderGeometry.GetLocation();
	Location.y += CurrentRenderGeometry.GetAllotedSpace().y;

	for (int32 i = 0; i <= Row; i++) {
		Location.y -= Rows[i]->GetValue(CurrentRenderGeometry);
	}
	for (int32 i = 0; i < Column; i++) {
		Location.x += Columns[i]->GetValue(CurrentRenderGeometry);
	}

	for (int32 i = Row; i < MathLibrary::Min(Row + RowSpan, Rows.Count()); i++) {
		Space.y += Rows[i]->GetValue(CurrentRenderGeometry);
	}
	for (int32 i = Column; i < MathLibrary::Min(Column + ColumnSpan, Columns.Count()); i++) {
		Space.x += Columns[i]->GetValue(CurrentRenderGeometry);
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

void PictorumGrid::Tick(float DeltaTime, const FRenderGeometry& Geometry) {
	TString debugLine = "Hovered Column: " + to_string(HoveredColumnIndex);
	debugLine += "\n";
	debugLine += "Hovered Row: " + to_string(HoveredRowIndex);
	debugLine += "\n";
	debugLine += "Distance To Wall: " + MathLibrary::LocationToString(DistanceToNearestEntry);
	debugLine += "\n";
	debugLine += "Cell Relative: " + MathLibrary::LocationToString(CellRelativePosition);
	FrameStatisticsWidget::SetExtraDebugString(debugLine);
}
void PictorumGrid::OnMouseEnter(const vec2& MousePosition, FUserInterfaceEvent& EventIn) {

}
void PictorumGrid::OnMouseExit(const vec2& MousePosition, FUserInterfaceEvent& EventIn) {
	// Set both values to -1 when the mouse leaves this container.
	HoveredColumnIndex = -1;
	HoveredRowIndex = -1;
}
void PictorumGrid::OnMouseMove(const vec2& MousePosition, const vec2& MouseDelta, FUserInterfaceEvent& EventIn) {
	if (WasClickedInside()) {
		int32 grow, shrink = 0;
		if (GetTargetResizeColumns(grow, shrink)) {
			Columns[grow]->Value += MouseDelta.x / LastRenderedGeometry.GetRenderResolution().x;
			Columns[shrink]->Value -= MouseDelta.x / LastRenderedGeometry.GetRenderResolution().x;
			EventIn.CaptureMouse();
		} else if (GetTargetResizeRows(grow, shrink)) {
			Rows[grow]->Value += -MouseDelta.y / LastRenderedGeometry.GetRenderResolution().y;
			Rows[shrink]->Value -= -MouseDelta.y / LastRenderedGeometry.GetRenderResolution().y;
			EventIn.CaptureMouse();
		}
	} else {
		CacheHoveredCellValues(MousePosition);
		int32 grow, shrink = 0;
		if (GetTargetResizeColumns(grow, shrink)) {
			Engine::GetInputSubsystem()->SetMouseCursorStyle(EMouseCursorStyle::SizeHorizontal);
		} else if (GetTargetResizeRows(grow, shrink)) {
			Engine::GetInputSubsystem()->SetMouseCursorStyle(EMouseCursorStyle::SizeVertical);
		} else {
			Engine::GetInputSubsystem()->SetMouseCursorStyle(EMouseCursorStyle::Arrow);
		}
	}
}
void PictorumGrid::OnMouseDown(const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& EventIn) {

}
void PictorumGrid::OnMouseUp(const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& EventIn) {

}
bool PictorumGrid::GetTargetResizeColumns(int32& GrowColumnIndex, int32& ShrinkColumnIndex) {
	GrowColumnIndex = -1;
	ShrinkColumnIndex = -1;
	if (DistanceToNearestEntry.x <= ResizeHandleDistance) {
		if (CellRelativePosition.x < 0.5) {
			GrowColumnIndex = HoveredColumnIndex - 1;
			ShrinkColumnIndex = HoveredColumnIndex;
		} else {
			GrowColumnIndex = HoveredColumnIndex;
			ShrinkColumnIndex = HoveredColumnIndex + 1;
		}
	}
	return GrowColumnIndex >= 0 && ShrinkColumnIndex >= 0 && GrowColumnIndex < Columns.Count() && ShrinkColumnIndex < Columns.Count() && ShrinkColumnIndex != GrowColumnIndex;
}
bool PictorumGrid::GetTargetResizeRows(int32& GrowRowIndex, int32& ShrinkRowIndex) {
	GrowRowIndex = -1;
	ShrinkRowIndex = -1;
	if (DistanceToNearestEntry.y <= ResizeHandleDistance) {
		if (CellRelativePosition.y < 0.5) {
			GrowRowIndex = HoveredRowIndex;
			ShrinkRowIndex = HoveredRowIndex + 1 ;
		} else {
			GrowRowIndex = HoveredRowIndex - 1;
			ShrinkRowIndex = HoveredRowIndex;
		}
	}
	return GrowRowIndex >= 0 && ShrinkRowIndex >= 0 && GrowRowIndex < Rows.Count() && ShrinkRowIndex < Rows.Count()  && ShrinkRowIndex != GrowRowIndex;
}
void PictorumGrid::CacheHoveredCellValues(const vec2& MousePosition) {
	for (int x = 0; x < Columns.Count(); x++) {
		for (int y = 0; y < Rows.Count(); y++) {
			Vector2D cellLocation, cellSpace, maxExtent;
			GetSlotDimensions(y, x, 1, 1, LastRenderedGeometry, cellLocation, cellSpace);
			maxExtent = cellLocation + cellSpace;

			if (MousePosition.x >= cellLocation.x && MousePosition.y >= cellLocation.y) {
				if (MousePosition.x <= maxExtent.x && MousePosition.y <= maxExtent.y) {

					Vector2D relativeMousePosition = MousePosition - cellLocation;
					HoveredColumnIndex = x;
					HoveredRowIndex = y;
					CellRelativePosition = relativeMousePosition / cellSpace;
					DistanceToNearestEntry = -abs(relativeMousePosition - cellSpace / 2.0f) + cellSpace / 2.0f;
					return;
				}
			}
		}
	}
}