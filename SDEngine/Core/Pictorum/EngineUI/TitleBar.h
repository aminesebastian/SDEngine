#pragma once
#include "Core/Pictorum/PictorumWidget.h"

class TitleBar : public PictorumWidget {
public:
	TitleBar(const TString& Name);
	virtual ~TitleBar();

	virtual bool CanAddChild() const override;
	virtual void CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const override;
};

