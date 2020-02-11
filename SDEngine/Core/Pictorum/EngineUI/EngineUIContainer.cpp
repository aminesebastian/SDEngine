#include "EngineUIContainer.h"
#include "Core/Pictorum/EngineUI/TitleBar.h"
#include "Core/Pictorum/EngineUI/FrameStatisticsWidget.h"
#include "Core/Pictorum/Widgets/LayoutWidget.h"

EngineUIContainer::EngineUIContainer(const TString& Name) : PictorumWidget(Name) {
	MainContainer   = nullptr;
	WindowTitleBar  = nullptr;
	FrameStatistics = nullptr;
}
EngineUIContainer::~EngineUIContainer() {

}
void EngineUIContainer::OnCreated() {
	MainContainer = new LayoutWidget("EngineUIContainer");
	AddChild(MainContainer);

	WindowTitleBar = new TitleBar("TitleBar");
	MainContainer->AddChild(WindowTitleBar);

	FrameStatistics = new FrameStatisticsWidget("FrameStatistics");
	MainContainer->AddChild(FrameStatistics);
}
const bool EngineUIContainer::CanAddChild() const {
	return true;
}