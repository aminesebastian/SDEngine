#include "FrameStatisticsWidget.h"
#include "Core/Pictorum/Widgets/LayoutWidget.h"
#include "Core/Pictorum/Widgets/TextWidget.h"
#include "Core/Utilities/StringUtilities.h"

FrameStatisticsWidget::FrameStatisticsWidget(const TString& Name) : PictorumWidget(Name) {
	FrameTimeWidget = nullptr;
	MaxFrameTimeCache = 100;
}
FrameStatisticsWidget::~FrameStatisticsWidget() {

}
void FrameStatisticsWidget::OnCreated() {
	LayoutWidget* mainLayout = new LayoutWidget("MainContainer");
	mainLayout->SetAnchor(EPictorumSide::LEFT, 1.0f);
	mainLayout->SetAnchor(EPictorumSide::TOP, 0.9f);
	mainLayout->SetAnchor(EPictorumSide::RIGHT, 1.0f);
	AddChild(mainLayout);

	FrameTimeWidget = new TextWidget("FrameTime");
	FrameTimeWidget->SetTextColor(FColor(0.0f, 1.0f, 0.1f));
	FrameTimeWidget->SetFontSize(12);
	FrameTimeWidget->SetTextAlignment(ETextAlignment::RIGHT);
	mainLayout->AddChild(FrameTimeWidget)->SetOffset(EPictorumSide::LEFT, -600);
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

	FrameTimeWidget->SetText(frameRate + " fps\n" + deltaTime + " ms" + "\nLorem ipsum dolor sit amet, consectetur\n adipiscing elit, sed do eiusmod tempor incididunt\nut labore et dolore magna aliqua. Ut enim ad minim veniam,\n quis nostrud exercitation ullamco laboris nisi ut \naliquip ex ea commodo consequat.\nLorem ipsum dolor sit amet, consectetur\n adipiscing elit, sed do eiusmod tempor incididunt\nut labore et dolore magna aliqua. Ut enim ad minim veniam,\n quis nostrud exercitation ullamco laboris nisi ut \naliquip ex ea commodo consequat.");
	//FrameTimeWidget->SetText(frameRate + " fps\n" + deltaTime + " ms");
}
float FrameStatisticsWidget::GetSmoothedFrameTime() {
	float total = 0.0f;
	for (float val : FrameTimeCache) {
		total += val;
	}
	return total / FrameTimeCache.Count();
}
