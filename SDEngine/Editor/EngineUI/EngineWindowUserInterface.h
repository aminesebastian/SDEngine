#pragma once
#include "Core/Pictorum/PictorumWidget.h"
#include "Core/Pictorum/Pictorum.h"
#include "Editor/EngineUI/EngineUIStyle.h"

class TitleBar;

class EngineWindowUserInterface : public PictorumWidget {
public:
	EngineWindowUserInterface(const TString& Name);
	virtual ~EngineWindowUserInterface();
	virtual void OnCreated() override;
	virtual const bool CanAddChild() const override;

protected:
	PictorumCanvas* MainContainer;

private:
	TitleBar* WindowTitleBar;
};

