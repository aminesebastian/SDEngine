#pragma once
#include "Core/Pictorum/PictorumWidget.h"

class TitleBar;
class LayoutWidget;

class EngineUIContainer : public PictorumWidget {
public:
	EngineUIContainer(const TString& Name);
	virtual ~EngineUIContainer();

	virtual const bool CanAddChild() const override;

private:
	LayoutWidget* MainContainer;
	TitleBar* WindowTitleBar;
};

