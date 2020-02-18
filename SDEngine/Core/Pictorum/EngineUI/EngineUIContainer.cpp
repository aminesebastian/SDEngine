#include "EngineUIContainer.h"
#include "Core/Pictorum/EngineUI/TitleBar.h"
#include "Core/Pictorum/EngineUI/FrameStatisticsWidget.h"
#include "Core/Pictorum/EngineUI/FloatingDetailsPanel.h"
#include "Core/Pictorum/Widgets/PictorumCanvas.h"
#include "Core/Pictorum/Widgets/SolidWidget.h"

EngineUIContainer::EngineUIContainer(const TString& Name) : PictorumWidget(Name) {
	MainContainer   = nullptr;
	WindowTitleBar  = nullptr;
	FrameStatistics = nullptr;
	DetailsPanel    = nullptr;
}
EngineUIContainer::~EngineUIContainer() {

}
void EngineUIContainer::OnCreated() {
	MainContainer = new PictorumCanvas("EngineUIContainer");
	AddChild(MainContainer);

	WindowTitleBar = new TitleBar("TitleBar");
	PictorumCanvasSlot* titleBarSlot = MainContainer->AddChild(WindowTitleBar);
	titleBarSlot->Anchors.SetTop(1.0f).SetBottom(1.0f);
	titleBarSlot->Offsets.SetYSize(30.0f).SetYPosition(-30.0f);

	FrameStatistics = new FrameStatisticsWidget("FrameStatistics");
	MainContainer->AddChild(FrameStatistics);

	DetailsPanel = new FloatingDetailsPanel("DetailsPanel");
	PictorumCanvasSlot* slot = MainContainer->AddChild(DetailsPanel);
	slot->Anchors.SetRight(1.0f).SetLeft(1.0f).SetTop(1.0f).SetBottom(0.0f);
	slot->Offsets.SetXPosition(-300.0f).SetXSize(300.0f).SetTop(30.0f);
}
const bool EngineUIContainer::CanAddChild() const {
	return true;
}