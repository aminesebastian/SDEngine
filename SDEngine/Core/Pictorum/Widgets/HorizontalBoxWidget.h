#pragma once
#include "Core/Pictorum/PictorumWidget.h"

class HorizontalBoxWidget : public PictorumWidget {
public:
	HorizontalBoxWidget(TString Name);
	virtual bool CanAddChild() const override;
	virtual void CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const override;
};

