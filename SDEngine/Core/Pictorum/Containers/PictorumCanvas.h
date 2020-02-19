#pragma once
#include "Core/Pictorum/PictorumWidget.h"
#include "Core/Pictorum/Slots/PictorumCanvasSlot.h"

class PictorumCanvas : public PictorumWidget {
public:
	PictorumCanvas(const TString& Name);
	virtual ~PictorumCanvas();

	virtual const bool CanAddChild() const override;
	virtual void CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const override;
	virtual PictorumCanvasSlot* AddChild(PictorumWidget* Widget) override;
	virtual PictorumCanvasSlot* CreateSlotForWidget(PictorumWidget* WidgetForSlot) const override;
	virtual bool PopulateDetailsPanel() override;
};

