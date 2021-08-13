#pragma once
#include "Core/Pictorum/PictorumWidget.h"
#include "Core/Pictorum/Slots/VerticalBoxSlot.h"

class PictorumVerticalBox : public PictorumWidget {
public:
	PictorumVerticalBox(const TString& EngineObjectName);
	virtual const bool CanAddChild() const override;
	virtual void CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const override;
	virtual vec2 GetDesiredDrawSpace(const FRenderGeometry& Geometry) const override;
	virtual VerticalBoxSlot* CreateSlotForWidget(PictorumWidget* WidgetForSlot) const override;
	virtual VerticalBoxSlot* AddChild(PictorumWidget* Widget) override;

	const FPadding& GetPadding() const;
	void SetPadding(const float& AllPadding);
	void SetPadding(const float& TopBottomPadding, const float& RightLeftPadding);
	void SetPadding(const float& Top, const float& Right, const float& Bottom, const float& Left);
protected:
	FPadding Padding;

	float GetFillRatioForChild(int32 ChildIndex) const;
	float GetNonFillSpaceRequirements(const FRenderGeometry& CurrentRenderGeometry) const;
	float CalculateYOffsetForChild(const FRenderGeometry& CurrentRenderGeometry, const int32& ChildIndex) const;
	float CalculateChildHeight(const FRenderGeometry& CurrentRenderGeometry, const int32 ChildIndex) const;
};

