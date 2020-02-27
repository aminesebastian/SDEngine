#pragma once
#include "Core/Pictorum/Pictorum.h"
#include "Core/Objects/Entities/Entity.h"
#include "Core/DataStructures/Array.h"

class FloatEditWidget;

class EntityInspector : public PictorumWidget {
public:
	EntityInspector(const TString& Name);
	virtual ~EntityInspector();
	virtual void OnCreated() override;

	void SetSelectedEntity(Entity* SelectedEntity);
private:
	Entity* DisplayedEntity;
	PictorumVerticalBox* DetailsPanelListBox;
};

