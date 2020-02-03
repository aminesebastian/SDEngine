#pragma once
#include "Core/Pictorum/PictorumWidget.h"

class HorizontalBoxWidget : public PictorumWidget {
public:
	HorizontalBoxWidget(TString Name);
	virtual void Tick(float DeltaTime, const FRenderGeometry& Geometry) override;

};

