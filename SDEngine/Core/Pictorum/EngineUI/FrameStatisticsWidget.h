#pragma once
#include "Core/Pictorum/PictorumWidget.h"

class TextWidget;

class FrameStatisticsWidget : public PictorumWidget {
public:
	FrameStatisticsWidget(const TString& Name);
	virtual ~FrameStatisticsWidget();

	virtual void Tick(float DeltaTime, const FRenderGeometry& Geometry) override;
	virtual void OnCreated() override;

private:
	TextWidget* FrameTimeWidget;
	SArray<float> FrameTimeCache;
	int MaxFrameTimeCache;

	float GetSmoothedFrameTime();
};

