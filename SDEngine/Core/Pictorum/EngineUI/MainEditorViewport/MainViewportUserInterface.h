#pragma once
#include "Core/Pictorum/PictorumWidget.h"
#include "Core/Pictorum/EngineUI/EngineWindowUserInterface.h"

class FrameStatisticsWidget;
class FloatingDetailsPanel;
class WorldOutlinerWidget;
class World;

class MainViewportUserInterface : public EngineWindowUserInterface {
public:
	MainViewportUserInterface(const TString& Name);
	virtual ~MainViewportUserInterface();
	virtual void OnCreated() override;

	void OnWorldUpdated(World* WorldIn);
private:
	FrameStatisticsWidget* FrameStatistics;
	FloatingDetailsPanel* DetailsPanel;
	WorldOutlinerWidget* WorldOutliner;
};



