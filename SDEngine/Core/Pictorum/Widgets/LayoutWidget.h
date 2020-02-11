#pragma once
#include "Core/Pictorum/PictorumWidget.h"
#include "Core/Pictorum/Slots/LayoutWidgetSlot.h"

class LayoutWidget : public PictorumWidget {
public:
	LayoutWidget(const TString& Name);
	virtual ~LayoutWidget();

	virtual const bool CanAddChild() const override;
	virtual void CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const override;
	virtual LayoutWidgetSlot* AddChild(PictorumWidget* Widget) override;
	virtual LayoutWidgetSlot* CreateSlotForWidget(PictorumWidget* WidgetForSlot) const override;
	virtual bool PopulateDetailsPanel() override;

	LayoutWidget* SetAnchor(const EPictorumSide& Side, const float& AnchorPosition);
private:
	FAnchors Anchors;
};

