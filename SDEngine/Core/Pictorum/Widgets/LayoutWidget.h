#pragma once
#include "Core/Pictorum/PictorumWidget.h"

class LayoutWidget : public PictorumWidget {
public:
	LayoutWidget(TString Name);
	~LayoutWidget();

	virtual void Tick(float DeltaTime, FRenderGeometry Geometry);

	virtual bool PopulateDetailsPanel() override;

private:
	FAnchors Anchors;

	void UpdateScale(FRenderGeometry Geometry);

};

