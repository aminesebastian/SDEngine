#pragma once
#include "Core/Pictorum/PictorumWidget.h"

class TitleBar;
class FrameStatisticsWidget;
class LayoutWidget;

class EngineUIContainer : public PictorumWidget {
public:
	EngineUIContainer(const TString& Name);
	virtual ~EngineUIContainer();
	virtual void OnCreated() override;
	virtual const bool CanAddChild() const override;

private:
	LayoutWidget* MainContainer;
	TitleBar* WindowTitleBar;
	FrameStatisticsWidget* FrameStatistics;
};

