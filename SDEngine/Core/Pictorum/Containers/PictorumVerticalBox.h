#pragma once
#include "Core/Pictorum/PictorumWidget.h"
#include "Core/Pictorum/Slots/VerticalBoxSlot.h"

class PictorumVerticalBox : public PictorumWidget {
public:
	PictorumVerticalBox(const TString& Name);
	virtual const bool CanAddChild() const override;
	virtual void CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const override;
	virtual vec2 GetDesiredDrawSpace(const FRenderGeometry& Geometry) const override;
	virtual VerticalBoxSlot* CreateSlotForWidget(PictorumWidget* WidgetForSlot) const override;
	virtual VerticalBoxSlot* AddChild(PictorumWidget* Widget) override;
protected:
	float GetFillRatioForChild(int32 ChildIndex) const;
	float GetNonFillSpaceRequirements(const FRenderGeometry& CurrentRenderGeometry) const;
	float CalculateYOffsetForChild(const FRenderGeometry& CurrentRenderGeometry, const int32& ChildIndex) const;
	float CalculateChildHeight(const FRenderGeometry& CurrentRenderGeometry, const int32 ChildIndex) const;
};

