#pragma once
#include "Core/Pictorum/PictorumWidget.h"
#include "Core/Pictorum/Slots/HorizontalBoxSlot.h"

class HorizontalBoxWidget : public PictorumWidget {
public:
	HorizontalBoxWidget(const TString& Name);
	virtual const bool CanAddChild() const override;
	virtual void CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const override;
	virtual vec2 GetDesiredDrawSpace(const FRenderGeometry& Geometry) const override;
	virtual HorizontalBoxSlot* CreateSlotForWidget(PictorumWidget* WidgetForSlot) const override;
	virtual HorizontalBoxSlot* AddChild(PictorumWidget* Widget) override;
protected:
	float GetFillRatioForChild(int32 ChildIndex) const;
	float GetNonFillSpaceRequirements(const FRenderGeometry& CurrentRenderGeometry) const;
	float CalculateXOffsetForChild(const FRenderGeometry& CurrentRenderGeometry, const int32& ChildIndex) const;
	float CalculateChildWidth(const FRenderGeometry& CurrentRenderGeometry, const int32 ChildIndex) const;
};

