#include "EngineUIContainer.h"
#include "Core/Pictorum/EngineUI/TitleBar.h"
#include "Core/Pictorum/Widgets/LayoutWidget.h"

EngineUIContainer::EngineUIContainer(const TString& Name) : PictorumWidget(Name) {
	MainContainer  = nullptr;
	WindowTitleBar = nullptr;
}
EngineUIContainer::~EngineUIContainer() {

}
void EngineUIContainer::OnCreated() {
	MainContainer = new LayoutWidget("MainContainer");
	AddChild(MainContainer);

	WindowTitleBar = new TitleBar("TitleBar");
	MainContainer->AddChild(WindowTitleBar);
}
const bool EngineUIContainer::CanAddChild() const {
	return true;
}