#pragma once
#include "Core/Pictorum/IWidgetSlot.h"

class VerticalBoxSlot : public IWidgetSlot {
public:
	VerticalBoxSlot();
	virtual ~VerticalBoxSlot();

	const EFillRule& GetFillRule() const;
	const float& GetFillRatio() const;
	const EVerticalAlignment& GetVerticalAlignment() const;
	const EHorizontalAlignment& GetHorizontalAlignment() const;

	VerticalBoxSlot& SetUseWidgetDesiredSpace();
	VerticalBoxSlot& SetFillAvilableSpace(const float& Ratio);
	VerticalBoxSlot& SetHorizontalAlignment(const EHorizontalAlignment& Alignment);
	VerticalBoxSlot& SetVerticalAlignment(const EVerticalAlignment& Alignment);

	FPadding& GetPadding();
private:
	FFillRule FillRule;
	FPadding Padding;
	EHorizontalAlignment HorizontalAlignment;
	EVerticalAlignment VerticalAlignment;
};

