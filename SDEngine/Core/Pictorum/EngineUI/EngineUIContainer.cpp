#include "EngineUIContainer.h"
#include "Core/Pictorum/Containers/PictorumCanvas.h"
#include "Core/Pictorum/Containers/PictorumGrid.h"
#include "Core/Pictorum/Containers/PictorumScrollBox.h"
#include "Core/Pictorum/Widgets/TextWidget.h"
#include "Core/Pictorum/EngineUI/EngineUIStyle.h"
#include "Core/Pictorum/EngineUI/FloatingDetailsPanel.h"
#include "Core/Pictorum/EngineUI/FrameStatisticsWidget.h"
#include "Core/Pictorum/EngineUI/TitleBar.h"
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
	
	PictorumGrid* grid = new PictorumGrid("Grid");
	grid->AddRow(0.5f);
	grid->AddRow(0.5f);
	grid->AddColumn(0.2f);
	grid->AddColumn(0.6f);
	grid->AddColumn(0.2f);
	PictorumCanvasSlot* canvasSlot = MainContainer->AddChild(grid);
	canvasSlot->Offsets.SetTop(30.0f);

	SolidWidget* leftSolid = new SolidWidget("leftSolid");
	leftSolid->SetBackgroundColor(EngineUIStyles::DARK_BACKGROUND_COLOR);
	PictorumGridSlot* leftSlot = grid->AddChild(leftSolid);
	leftSlot->Column = 0;
	leftSlot->Row = 0;
	leftSlot->RowSpan = 2;

	FrameStatistics = new FrameStatisticsWidget("FrameStatistics");
	PictorumGridSlot* middleSlot = grid->AddChild(FrameStatistics);
	middleSlot->Column = 1;
	middleSlot->Row = 0;
	middleSlot->RowSpan = 2;

	PictorumScrollBox* scrollBox = new PictorumScrollBox("TestScrollBox");
	PictorumGridSlot* scrollSlot = grid->AddChild(scrollBox);
	scrollSlot->Column = 2;
	scrollSlot->Row = 0;
	scrollSlot->RowSpan = 1;


	for (int i = 0; i < 200; i++) {
		TextWidget* text = new TextWidget("test" + to_string(i));
		text->SetText("Test entry: " + to_string(i));
		text->SetFontSize(12);
		scrollBox->AddChild(text);
	}


	DetailsPanel = new FloatingDetailsPanel("DetailsPanel");
	PictorumGridSlot* rightSlot = grid->AddChild(DetailsPanel);
	rightSlot->Column = 2;
	rightSlot->Row = 1;
	rightSlot->RowSpan = 1;
}
const bool EngineUIContainer::CanAddChild() const {
	return true;
}