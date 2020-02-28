#pragma once
#include "Core/Pictorum/PictorumWidget.h"
#include "Editor/EngineUI/WorldOutliner/WorldOutlinerEntry.h"
#include "Core/Engine/World.h"

class PictorumScrollBox;
class World;

class WorldOutlinerWidget : public PictorumWidget {
public:
	WorldOutlinerWidget(const TString& Name);
	virtual ~WorldOutlinerWidget();

	virtual void OnCreated() override;
	void SetWorld(World* WorldIn);

	Event<void(const SArray<Entity*>&)> SelectedEntitiesUpdatedDelegate;
protected:
	PictorumScrollBox* ScrollBox;
	World* ReferencedWorld;

	void UpdateEntityList();
	void OnSelectedValuesChanged(PictorumWidget* ScrollBoxWidget, const SArray<int32>& SelectedIndices);
};

