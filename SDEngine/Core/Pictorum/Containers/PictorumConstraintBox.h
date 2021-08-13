#pragma once
#include "Core/Pictorum/PictorumWidget.h"
#include "Core/Pictorum/Slots/ConstraintBoxSlot.h"

class PictorumConstraintBox : public PictorumWidget {
public:
	PictorumConstraintBox(const TString& Name);
	virtual const bool CanAddChild() const override;
	virtual void CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const override;
	//virtual Vector2D GetDesiredDrawSpace(const FRenderGeometry& Geometry) const override;

	virtual ConstraintBoxSlot* CreateSlotForWidget(PictorumWidget* WidgetForSlot) const override;
	virtual ConstraintBoxSlot* AddChild(PictorumWidget* Widget) override;

	PictorumConstraintBox& SetSize(const vec2& Size);
	PictorumConstraintBox& SetMinimumSize(const vec2& Size);
	PictorumConstraintBox& SetMaximumSize(const vec2& Size);
	PictorumConstraintBox& SetScaleMultiplier(const float& Multiplier);
	PictorumConstraintBox& SetMaintainAspectRatio(bool MaintainAspectRatio);

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

