#pragma once
#include "Core/Pictorum/PictorumWidget.h"
#include "Core/Objects/Entities/Entity.h"
#include "Editor/UserInterface/EngineWindowUserInterface.h"

class FrameStatisticsWidget;
class EntityInspector;
class WorldOutlinerWidget;
class World;

class MainViewportUserInterface : public EngineWindowUserInterface {
public:
	MainViewportUserInterface(const TString& Name);
	virtual ~MainViewportUserInterface();
	virtual void OnCreated() override;

	void OnWorldUpdated(World* WorldIn);
	void OnSelectedEntitesChanged(const SArray<Entity*>& SelectedEntities);
private:
	FrameStatisticsWidget* FrameStatistics;
	EntityInspector* DetailsPanel;
	WorldOutlinerWidget* WorldOutliner;
};



