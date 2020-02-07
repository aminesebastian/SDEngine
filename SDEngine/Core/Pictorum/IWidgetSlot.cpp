#include "IWidgetSlot.h"

void IWidgetSlot::ModifyGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry) {
	OutputGeometry = CurrentRenderGeometry;
}