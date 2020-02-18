#pragma once
#include "Core/Pictorum/PictorumWidget.h"

class TitleBar;
class FrameStatisticsWidget;
class PictorumCanvas;
class FloatingDetailsPanel;

class EngineUIContainer : public PictorumWidget {
public:
	EngineUIContainer(const TString& Name);
	virtual ~EngineUIContainer();
	virtual void OnCreated() override;
	virtual const bool CanAddChild() const override;

private:
	PictorumCanvas* MainContainer;
	TitleBar* WindowTitleBar;
	FrameStatisticsWidget* FrameStatistics;
	FloatingDetailsPanel* DetailsPanel;
};

