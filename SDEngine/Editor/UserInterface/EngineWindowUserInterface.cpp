#include "EngineWindowUserInterface.h"
#include "Core/Pictorum/Containers/PictorumCanvas.h"
#include "Editor/UserInterface/TitleBar.h"

EngineWindowUserInterface::EngineWindowUserInterface(const TString& Name) : PictorumWidget(Name) {
	MainContainer = nullptr;
	WindowTitleBar = nullptr;
}
EngineWindowUserInterface::~EngineWindowUserInterface() {

}
void EngineWindowUserInterface::OnCreated() {
	MainContainer = new PictorumCanvas(GetObjectName() + "WindowUserInterface");
	AddChild(MainContainer);

	WindowTitleBar = new TitleBar(GetObjectName() + "WindowTitleBar");
	PictorumCanvasSlot* titleBarSlot = MainContainer->AddChild(WindowTitleBar);
	titleBarSlot->Anchors.SetTop(1.0f).SetBottom(1.0f);
	titleBarSlot->Offsets.SetYSize(30.0f).SetYPosition(-30.0f);
}
const bool EngineWindowUserInterface::CanAddChild() const {
	return true;
}