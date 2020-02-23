#pragma once
#include "Core/Pictorum/PictorumWidget.h"
#include "Core/Engine/World.h"

class PictorumScrollBox;
class World;

class WorldOutlinerWidget : public PictorumWidget {
public:
	WorldOutlinerWidget(const TString& Name);
	virtual ~WorldOutlinerWidget();

	virtual void OnCreated() override;

	void WorldUpdated(World* WorldIn);
	void OnSelectedValuesChanged(PictorumWidget* ScrollBoxWidget, const SArray<int32>& SelectedIndices);
private:
	PictorumScrollBox* ScrollBox;
	World* ReferencedWorld;
};

