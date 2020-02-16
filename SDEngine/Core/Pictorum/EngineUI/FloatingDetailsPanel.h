#pragma once
#include "Core/Pictorum/PictorumWidget.h"
#include "Core/DataStructures/Array.h"

class FloatEditWidget;

class FloatingDetailsPanel : public PictorumWidget {
public:
	FloatingDetailsPanel(const TString& Name);
	virtual ~FloatingDetailsPanel();
	virtual void OnCreated() override;

private:

};

