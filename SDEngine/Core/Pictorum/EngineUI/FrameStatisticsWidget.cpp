#include "FrameStatisticsWidget.h"
#include "Core/Pictorum/Containers/PictorumCanvas.h"
#include "Core/Pictorum/Widgets/TextWidget.h"
#include "Core/Utilities/StringUtilities.h"
#include "Core/Utilities/Math/MathLibrary.h"
#include "Core/Input/InputSubsystem.h"
#include "Core/Pictorum/PictorumRenderer.h"

FrameStatisticsWidget::FrameStatisticsWidget(const TString& Name) : PictorumWidget(Name) {
	FrameTimeWidget = nullptr;
	MaxFrameTimeCache = 100;
}
FrameStatisticsWidget::~FrameStatisticsWidget() {

}
void FrameStatisticsWidget::OnCreated() {
	PictorumCanvas* mainLayout = new PictorumCanvas("MainContainer");
	AddChild(mainLayout);

	FrameTimeWidget = new TextWidget("FrameTime");
	FrameTimeWidget->SetTextColor(FColor(0.0f, 1.0f, 0.1f));
	FrameTimeWidget->SetFontSize(12);
	FrameTimeWidget->SetTextAlignment(ETextAlignment::RIGHT);
	PictorumCanvasSlot* slot = mainLayout->AddChild(FrameTimeWidget);
	slot->Anchors.SetRight(1.0f).SetLeft(1.0f).SetTop(1.0f).SetBottom(0.0f);
	slot->Offsets.SetXPosition(-900.0f).SetXSize(900.0f).SetTop(40.0f);
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
	TString text = frameRate + "fps\n";
	text += deltaTime + "ms\n";
	text += "Mouse Position: " + MathLibrary::LocationToString(Engine::Get()->GetInputSubsystem()->GetMousePosition()) +"\n";

	if (GetOwningRenderer()->GetHoveredWidget()) {
		text += "Hovered Widget: " + GetOwningRenderer()->GetHoveredWidget()->GetName();
	} else {
		text += "Hovered Widget: None";
	}

	FrameTimeWidget->SetText(text);
}
float FrameStatisticsWidget::GetSmoothedFrameTime() {
	float total = 0.0f;
	for (float val : FrameTimeCache) {
		total += val;
	}
	return total / FrameTimeCache.Count();
}
