#include "MainViewportUserInterface.h"
#include "Core/Pictorum/Containers/PictorumGrid.h"
#include "Core/Pictorum/EngineUI/FrameStatisticsWidget.h"
#include "Core/Pictorum/EngineUI/FloatingDetailsPanel.h"
#include "Core/Pictorum/EngineUI/WorldOutliner/WorldOutlinerWidget.h"
#include "Core/Engine/World.h"

MainViewportUserInterface::MainViewportUserInterface(const TString& Name) : EngineWindowUserInterface(Name) {
	FrameStatistics = nullptr;
	DetailsPanel    = nullptr;
	WorldOutliner   = nullptr;
}
MainViewportUserInterface::~MainViewportUserInterface() {

}
void MainViewportUserInterface::OnCreated() {
	EngineWindowUserInterface::OnCreated();

	PictorumGrid* grid = new PictorumGrid("Grid");
	grid->AddRow(0.25f);
	grid->AddRow(0.75f);
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

	WorldOutliner = new WorldOutlinerWidget("WorldOutliner");
	PictorumGridSlot* scrollSlot = grid->AddChild(WorldOutliner);
	scrollSlot->Column = 2;
	scrollSlot->Row = 0;
	scrollSlot->RowSpan = 1;

	DetailsPanel = new FloatingDetailsPanel("DetailsPanel");
	PictorumGridSlot* rightSlot = grid->AddChild(DetailsPanel);
	rightSlot->Column = 2;
	rightSlot->Row = 1;
	rightSlot->RowSpan = 1;

	WorldOutliner->SelectedEntitiesUpdatedDelegate.Add<MainViewportUserInterface, & MainViewportUserInterface::OnSelectedEntitesChanged>(this);
}
void MainViewportUserInterface::OnWorldUpdated(World* WorldIn) {
	WorldOutliner->SetWorld(WorldIn);
}
void MainViewportUserInterface::OnSelectedEntitesChanged(const SArray<Entity*>& SelectedEntities) {
	DetailsPanel->SetSelectedEntity(SelectedEntities[0]);
}