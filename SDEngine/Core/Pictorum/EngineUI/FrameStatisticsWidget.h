#pragma once
#include "Core/Pictorum/PictorumWidget.h"

class TextWidget;
static TString ExtraDebugString;
class FrameStatisticsWidget : public PictorumWidget {
public:
	FrameStatisticsWidget(const TString& Name);
	virtual ~FrameStatisticsWidget();

	virtual void Tick(float DeltaTime, const FRenderGeometry& Geometry) override;
	virtual void OnCreated() override;

	static void SetExtraDebugString(const TString& DebugString);
private:
	TextWidget* FrameTimeWidget;
	SArray<float> FrameTimeCache;
	int MaxFrameTimeCache;

	float GetSmoothedFrameTime();


};

