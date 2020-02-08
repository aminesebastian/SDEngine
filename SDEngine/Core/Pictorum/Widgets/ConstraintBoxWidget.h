#pragma once
#include "Core/Pictorum/PictorumWidget.h"
#include "Core/Pictorum/Slots/ConstraintBoxSlot.h"

class ConstraintBoxWidget : public PictorumWidget {
public:
	ConstraintBoxWidget(const TString& Name);
	virtual bool CanAddChild() const override;
	virtual void CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const override;
	virtual ConstraintBoxSlot* CreateSlotForWidget(PictorumWidget* WidgetForSlot) const override;
	virtual ConstraintBoxSlot* AddChild(PictorumWidget* Widget) override;

	ConstraintBoxWidget& SetSize(const vec2& Size);
	ConstraintBoxWidget& SetMinimumSize(const vec2& Size);
	ConstraintBoxWidget& SetMaximumSize(const vec2& Size);
	ConstraintBoxWidget& SetScaleMultiplier(const float& Multiplier);
	ConstraintBoxWidget& SetMaintainAspectRatio(bool MaintainAspectRatio);

	const vec2& GetMinimumSize() const;
	const vec2& GetMaximumSize() const;
	const float& GetScaleMultiplier() const;
	const bool& GetMaintainAspectRatio() const;
protected:
	vec2 MaximumSize;
	vec2 MinimumSize;
	float ScaleMultiplier;
	bool bMaintainAspectRatio;
};

