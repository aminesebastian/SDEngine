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

void PictorumGrid::Draw(float DeltaTime, const FRenderGeometry& Geometry) {
	for (PictorumWidget* child : Children) {
		PictorumGridSlot* slot = child->GetParentSlot<PictorumGridSlot>();
		if (HoveredRowIndex == slot->Row && HoveredColumnIndex == slot->Column && DistanceToNearestEntry.x <= ResizeHandleDistance) {
			Vector2D slotLocation, slotSize;
			GetSlotDimensions(slot->Row, slot->Column, slot->RowSpan, slot->ColumnSpan, Geometry, slotLocation, slotSize);
			FBoxDrawInstruction drawInstruction;
			drawInstruction.Size.x = 2.0f;
			drawInstruction.BackgroundColor = FColor(0.4f, 0.4f, 0.475f);
			drawInstruction.Size.y = slotSize.y;
			drawInstruction.Location = slotLocation;

			drawInstruction.Location.x = slotLocation.x - 2.0f;
			DrawQuad(Geometry, drawInstruction);

			drawInstruction.Location.x = slotLocation.x + slotSize.x;
			DrawQuad(Geometry, drawInstruction);
		}
	}
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
	Location = CurrentRenderGeometry.GetLocation();
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
	// If the mouse was clicked inside this widget, check if we are in resizing range.
	// Otherwise, cache the data required for making this determination.
	if (WasClickedInside()) {
		Vector2D relativeDelta = MouseDelta / LastRenderedGeometry.GetRenderResolution();
		int32 grow, shrink = 0;
		if (GetTargetResizeColumns(grow, shrink)) {
			int32 actualShrink = relativeDelta.x > 0 ? shrink : grow;
			if (GetColumnMinSize(actualShrink) <= Columns[actualShrink]->Value - relativeDelta.x) {
				Columns[grow]->Value += relativeDelta.x;
				Columns[shrink]->Value -= relativeDelta.x;
				EventIn.CaptureMouse();
			}
		} else if (GetTargetResizeRows(grow, shrink)) {
			int32 actualShrink = -relativeDelta.y > 0 ? shrink : grow;
			if (GetRowMinSize(actualShrink) <= Rows[actualShrink]->Value + relativeDelta.y) {
				Rows[grow]->Value += -relativeDelta.y;
				Rows[shrink]->Value -= -relativeDelta.y;
				EventIn.CaptureMouse();
			}
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
PictorumWidget* PictorumGrid::GetWidgetInSlot(const int32& Row, const int32& Column) {
	for (PictorumWidget* child : Children) {
		PictorumGridSlot* slot = child->GetParentSlot<PictorumGridSlot>();
		if (slot) {
			if (slot->Column == Column && slot->Row == Row) {
				return child;
			}
		}
	}
	return nullptr;
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
			ShrinkRowIndex = HoveredRowIndex + 1;
		} else {
			GrowRowIndex = HoveredRowIndex - 1;
			ShrinkRowIndex = HoveredRowIndex;
		}
	}
	return GrowRowIndex >= 0 && ShrinkRowIndex >= 0 && GrowRowIndex < Rows.Count() && ShrinkRowIndex < Rows.Count() && ShrinkRowIndex != GrowRowIndex;
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
const float PictorumGrid::GetRowMinSize(const int32& RowIndex) {
	float relativeValue = 0.0f;
	for (int32 i = 0; i < Columns.Count(); i++) {
		PictorumWidget* widget = GetWidgetInSlot(RowIndex, i);
		relativeValue += widget->GetDesiredDrawSpace(LastRenderedGeometry).y;
	}
	return relativeValue / LastRenderedGeometry.GetAllotedSpace().y;
}
const float PictorumGrid::GetColumnMinSize(const int32& ColumnIndex) {
	float relativeValue = 0.0f;
	for (int32 i = 0; i < Rows.Count(); i++) {
		PictorumWidget* widget = GetWidgetInSlot(i, ColumnIndex);
		relativeValue += widget->GetDesiredDrawSpace(LastRenderedGeometry).x;
	}
	return relativeValue / LastRenderedGeometry.GetAllotedSpace().x;
}