#include "FrameStatisticsWidget.h"
#include "Core/Pictorum/Widgets/LayoutWidget.h"
#include "Core/Pictorum/Widgets/TextWidget.h"

FrameStatisticsWidget::FrameStatisticsWidget(const TString& Name) : PictorumWidget(Name){
}
FrameStatisticsWidget::~FrameStatisticsWidget() {

}
void FrameStatisticsWidget::OnCreated() {
	LayoutWidget* mainLayout = new LayoutWidget("MainContainer");
	AddChild(mainLayout);

	FrameTimeWidget = new TextWidget("FrameTime");
	FrameTimeWidget->SetTextColor(FColor(0.0f, 1.0f, 0.1f));
	mainLayout->AddChild(FrameTimeWidget)->SetOffset(EPictorumSide::TOP, 0.3);
}
void FrameStatisticsWidget::Tick(float DeltaTime, const FRenderGeometry& Geometry) {
	PictorumWidget::Tick(DeltaTime, Geometry);
	FrameTimeWidget->SetText(DeltaTime + "ms")
}