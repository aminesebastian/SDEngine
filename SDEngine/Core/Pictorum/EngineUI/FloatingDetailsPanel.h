#pragma once
#include "Core/Pictorum/Pictorum.h"
#include "Core/Objects/Entities/Entity.h"
#include "Core/DataStructures/Array.h"

class FloatEditWidget;

class FloatingDetailsPanel : public PictorumWidget {
public:
	FloatingDetailsPanel(const TString& Name);
	virtual ~FloatingDetailsPanel();
	virtual void OnCreated() override;

	void SetSelectedEntity(Entity* SelectedEntity);
private:
	Entity* DisplayedEntity;
	PictorumVerticalBox* DetailsPanelListBox;
};

