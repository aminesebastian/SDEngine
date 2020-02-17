#include "EngineUIContainer.h"
#include "Core/Pictorum/EngineUI/TitleBar.h"
#include "Core/Pictorum/EngineUI/FrameStatisticsWidget.h"
#include "Core/Pictorum/EngineUI/FloatingDetailsPanel.h"
#include "Core/Pictorum/Widgets/LayoutWidget.h"
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
	MainContainer = new LayoutWidget("EngineUIContainer");
	AddChild(MainContainer);

	WindowTitleBar = new TitleBar("TitleBar");
	LayoutWidgetSlot* titleBarSlot = MainContainer->AddChild(WindowTitleBar);
	titleBarSlot->SetAnchor(EPictorumSide::BOTTOM, 1.0f);
	titleBarSlot->SetOffset(EPictorumSide::TOP, 30.0f);
	titleBarSlot->SetPivotOffset(0.0f, -1.0f);

	//FrameStatistics = new FrameStatisticsWidget("FrameStatistics");
	//MainContainer->AddChild(FrameStatistics);

	DetailsPanel = new FloatingDetailsPanel("DetailsPanel");
	LayoutWidgetSlot* slot = MainContainer->AddChild(DetailsPanel);
	slot->SetAnchor(EPictorumSide::LEFT, 1.0f);
	slot->SetOffset(EPictorumSide::LEFT, -300.0f);
}
const bool EngineUIContainer::CanAddChild() const {
	return true;
}