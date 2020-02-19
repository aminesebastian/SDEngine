#include "EngineUIContainer.h"
#include "Core/Pictorum/EngineUI/TitleBar.h"
#include "Core/Pictorum/EngineUI/FrameStatisticsWidget.h"
#include "Core/Pictorum/EngineUI/FloatingDetailsPanel.h"
#include "Core/Pictorum/Containers/PictorumCanvas.h"
#include "Core/Pictorum/Containers/PictorumGrid.h"
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

	//DetailsPanel = new FloatingDetailsPanel("DetailsPanel");
	//PictorumCanvasSlot* slot = MainContainer->AddChild(DetailsPanel);
	//slot->Anchors.SetRight(1.0f).SetLeft(1.0f).SetTop(1.0f).SetBottom(0.0f);
	//slot->Offsets.SetXPosition(-300.0f).SetXSize(300.0f).SetTop(30.0f);
	
	PictorumGrid* grid = new PictorumGrid("Grid");
	grid->AddRow(0.5f, true);
	grid->AddRow(0.5f, true);
	grid->AddColumn(0.5f, true);
	grid->AddColumn(0.5f, true);
	MainContainer->AddChild(grid);

	SolidWidget* topLeft = new SolidWidget("topLeft");
	topLeft->SetBackgroundColor(FColor(1.0f, 0.0f, 0.0f));
	SolidWidget* topRight = new SolidWidget("topRight");
	topRight->SetBackgroundColor(FColor(0.0f, 1.0f, 0.0f));
	SolidWidget* bottomLeft = new SolidWidget("bottomLeft");
	bottomLeft->SetBackgroundColor(FColor(0.0f, 0.0f, 1.0f));
	SolidWidget* bottomRight = new SolidWidget("bottomRight");
	bottomRight->SetBackgroundColor(FColor(1.0f, 0.0f, 1.0f));

	PictorumGridSlot* topLeftSlot = grid->AddChild(topLeft);
	topLeftSlot->Column = 0;
	topLeftSlot->Row = 0;

	PictorumGridSlot* topRightSlot = grid->AddChild(topRight);
	topRightSlot->Column = 1;
	topRightSlot->Row = 0;

	PictorumGridSlot* bottomLeftSlot = grid->AddChild(bottomLeft);
	bottomLeftSlot->Column = 0;
	bottomLeftSlot->Row = 1;

	PictorumGridSlot* bottomRightSlot = grid->AddChild(bottomRight);
	bottomRightSlot->Column = 1;
	bottomRightSlot->Row = 1;
}
const bool EngineUIContainer::CanAddChild() const {
	return true;
}