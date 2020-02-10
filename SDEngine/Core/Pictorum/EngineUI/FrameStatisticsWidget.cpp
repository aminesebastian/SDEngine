#include "FrameStatisticsWidget.h"
#include "Core/Pictorum/Widgets/LayoutWidget.h"
#include "Core/Pictorum/Widgets/TextWidget.h"
#include "Core/Utilities/StringUtilities.h"

FrameStatisticsWidget::FrameStatisticsWidget(const TString& Name) : PictorumWidget(Name) {
	FrameTimeWidget   = nullptr;
	MaxFrameTimeCache = 100;
}
FrameStatisticsWidget::~FrameStatisticsWidget() {

}
void FrameStatisticsWidget::OnCreated() {
	LayoutWidget* mainLayout = new LayoutWidget("MainContainer");
	AddChild(mainLayout);

	FrameTimeWidget = new TextWidget("FrameTime");
	FrameTimeWidget->SetTextColor(FColor(0.0f, 1.0f, 0.1f));
	FrameTimeWidget->SetFontSize(12);
	mainLayout->AddChild(FrameTimeWidget)->SetOffset(EPictorumSide::TOP, 0.9f)->SetOffset(EPictorumSide::LEFT, -100)->SetOffsetAbsolute(EPictorumSide::LEFT);
}
void FrameStatisticsWidget::Tick(float DeltaTime, const FRenderGeometry& Geometry) {
	PictorumWidget::Tick(DeltaTime, Geometry);

	FrameTimeCache.Add(DeltaTime);
	if (FrameTimeCache.Count() > MaxFrameTimeCache) {
		FrameTimeCache.RemoveAt(0);
	}

	float smootedFrameTime = GetSmoothedFrameTime();
	TString deltaTime = StringUtilities::ToStringWithPrecision(smootedFrameTime * 1000.0f, 2);
	TString frameRate = StringUtilities::ToStringWithPrecision(1.0f / smootedFrameTime, 2);

	FrameTimeWidget->SetText(deltaTime + " ms\n" + frameRate + " fps");
}
float FrameStatisticsWidget::GetSmoothedFrameTime() {
	float total = 0.0f;
	for (float val : FrameTimeCache) {
		total += val;
	}
	return total / FrameTimeCache.Count();
}
