#pragma once
#include "Core/Pictorum/PictorumWidget.h"

class LayoutWidget : public PictorumWidget {
public:
	LayoutWidget(TString Name);
	~LayoutWidget();

	virtual void CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) override;

	virtual bool PopulateDetailsPanel() override;
private:
	FAnchors Anchors;
};

