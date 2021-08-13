#include "MainViewportUserInterface.h"
#include "Core/Engine/World.h"
#include "Core/Pictorum/Containers/PictorumGrid.h"
#include "Editor/UserInterface/FrameStatisticsWidget.h"
#include "Editor/UserInterface/Inspector/EntityInspectorWidget.h"
#include "Editor/UserInterface/WorldOutliner/WorldOutlinerWidget.h"
#include "Editor/MultiLineTestWidget.h"

MainViewportUserInterface::MainViewportUserInterface(const TString& Name) : EngineWindowUserInterface(Name) {
	FrameStatistics = nullptr;
	DetailsPanel = nullptr;
	WorldOutliner = nullptr;
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

	MultiLineTestWidget* test = new MultiLineTestWidget("Test");
	//SolidWidget* leftSolid = new SolidWidget("leftSolid");
	//leftSolid->SetBackgroundColor(EngineUIStyles::DARK_BACKGROUND_COLOR);
	PictorumGridSlot* leftSlot = grid->AddChild(test);
	leftSlot->Column = 0;
	leftSlot->Row = 0;
	leftSlot->RowSpan = 2;

	//FrameStatistics = new FrameStatisticsWidget("FrameStatistics");
	//PictorumGridSlot* middleSlot = grid->AddChild(FrameStatistics);
	//middleSlot->Column = 1;
	//middleSlot->Row = 0;
	//middleSlot->RowSpan = 2;

	WorldOutliner = new WorldOutlinerWidget("WorldOutliner");
	PictorumGridSlot* scrollSlot = grid->AddChild(WorldOutliner);
	scrollSlot->Column = 2;
	scrollSlot->Row = 0;
	scrollSlot->RowSpan = 1;

	DetailsPanel = new EntityInspector("DetailsPanel");
	PictorumGridSlot* rightSlot = grid->AddChild(DetailsPanel);
	rightSlot->Column = 2;
	rightSlot->Row = 1;
	rightSlot->RowSpan = 1;

	WorldOutliner->SelectedEntitiesUpdatedDelegate.Add<MainViewportUserInterface, & MainViewportUserInterface::OnSelectedEntitesChanged>(this);
}
void MainViewportUserInterface::OnWorldUpdated(World* WorldIn) {
	WorldOutliner->SetWorld(WorldIn);
	if (WorldIn->GetSelectedEntity()) {
		DetailsPanel->SetSelectedEntity(WorldIn->GetSelectedEntity());
	}
}
void MainViewportUserInterface::OnSelectedEntitesChanged(const SArray<Entity*>& SelectedEntities) {
	DetailsPanel->SetSelectedEntity(SelectedEntities[0]);
}