#include "PictorumCanvas.h"
#include "Core/Utilities/EngineFunctionLibrary.h"
#include "Core/Utilities/Logger.h"
#include "Core/Utilities/Math/MathLibrary.h"
#include "UserInterface/Widgets/DragFloat.h"

PictorumCanvas::PictorumCanvas(const TString& Name) : PictorumWidget(Name) {
	SetVisibility(EPictorumVisibilityState::SELF_HIT_TEST_INVISIBLE);
}
PictorumCanvas::~PictorumCanvas() {

}

const bool PictorumCanvas::CanAddChild() const {
	return true;
}
void PictorumCanvas::CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const {
	PictorumCanvasSlot* slot = GetChildSlotAtIndex<PictorumCanvasSlot>(ChildIndex);

	const FAnchors& anchors = slot->Anchors;
	const FOffsets& offsets = slot->Offsets;
	const Vector2D& res	    = CurrentRenderGeometry.GetRenderResolution();

	Vector2D originalLocation = CurrentRenderGeometry.GetLocation();
	Vector2D originalScale = CurrentRenderGeometry.GetAllotedSpace();
	Vector2D originalResolution = originalLocation + originalScale;

	Vector2D newLocation;
	Vector2D newSpace;

	if (anchors.GetLeft() == anchors.GetRight()) { // Left or right screen aligned.

		newLocation.x = originalLocation.x + (originalResolution.x * anchors.GetLeft()) + offsets.GetXPosition();
		newSpace.x	  = offsets.GetXSize();
		newLocation.y = originalLocation.y + (originalResolution.y * anchors.GetBottom()) + offsets.GetBottom();
		newSpace.y    = (originalResolution.y * anchors.GetTop()) - offsets.GetBottom() - offsets.GetTop();

	} else if (anchors.GetTop() == anchors.GetBottom()) { // Top or bottom screen aligned.

		newLocation.y = originalLocation.y + (originalResolution.y * anchors.GetTop()) + offsets.GetYPosition();
		newSpace.y = offsets.GetYSize();
		newLocation.x = originalLocation.x + (originalResolution.x * anchors.GetLeft()) + offsets.GetLeft();
		newSpace.x = (originalResolution.x * anchors.GetRight()) - offsets.GetLeft() - offsets.GetRight();

	} else if(anchors.GetLeft() == anchors.GetTop()) { // Centered around a point.

		newLocation.x = originalLocation.x + (originalResolution.x * anchors.GetLeft()) + offsets.GetXPosition();
		newSpace.x = offsets.GetXSize();
		newLocation.y = originalLocation.y + (originalResolution.y * anchors.GetBottom()) + offsets.GetYPosition();
		newSpace.y = offsets.GetYSize();

	} else { // A rectangular area.
		newLocation.x = originalLocation.x + (originalResolution.x * anchors.GetLeft()) + offsets.GetLeft();
		newSpace.x = originalResolution.x - offsets.GetLeft() - offsets.GetRight();
		newLocation.y = originalLocation.y + (originalResolution.y * anchors.GetBottom()) + offsets.GetBottom();
		newSpace.y = originalResolution.y - offsets.GetTop() - offsets.GetBottom();
	}

	OutputGeometry.SetLocation(newLocation);
	OutputGeometry.SetAllotedSpace(newSpace);
	OutputGeometry.SetMaximumClipPoint(OutputGeometry.GetLocation() + OutputGeometry.GetAllotedSpace());
	OutputGeometry.SetMinimumClipPoint(OutputGeometry.GetLocation());
}

PictorumCanvasSlot* PictorumCanvas::AddChild(PictorumWidget* Widget) {
	return Cast<PictorumCanvasSlot>(AddChildInternal(Widget));
}
PictorumCanvasSlot* PictorumCanvas::CreateSlotForWidget(PictorumWidget* WidgetForSlot) const {
	return new PictorumCanvasSlot();
}
bool PictorumCanvas::PopulateDetailsPanel() {
	PictorumWidget::PopulateDetailsPanel();

	return true;
}
