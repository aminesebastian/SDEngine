#include "WorldOutlinerWidget.h"
#include "Core/Engine/World.h"
#include "Core/Objects/Entities/Actor.h"
#include "Core/Pictorum/Widgets/TextWidget.h"
#include "Core/Pictorum/Widgets/SolidWidget.h"
#include "Editor/EngineUI/EngineUIStyle.h"
#include "Core/Pictorum/Containers/PictorumScrollBox.h"

WorldOutlinerWidget::WorldOutlinerWidget(const TString& Name) : PictorumWidget(Name) {
	ReferencedWorld = nullptr;
	ScrollBox       = nullptr;
}
WorldOutlinerWidget::~WorldOutlinerWidget() {

}

void WorldOutlinerWidget::OnCreated() {
	SolidWidget* bg = new SolidWidget("WorldOutlinerBackground");
	AddChild(bg);
	bg->SetBackgroundColor(EngineUIStyles::BACKGROUND_COLOR);

	ScrollBox = new PictorumScrollBox("WorldOutlinerScrollBox");
	bg->AddChild(ScrollBox);
	ScrollBox->SelectionUpdated.Add<WorldOutlinerWidget, &WorldOutlinerWidget::OnSelectedValuesChanged>(this);
}
void WorldOutlinerWidget::SetWorld(World* WorldIn) {
	ReferencedWorld = WorldIn;
	UpdateEntityList();
}
void WorldOutlinerWidget::UpdateEntityList() {
	ScrollBox->ClearChildren();

	for (Actor* actor : ReferencedWorld->GetWorldActors()) {
		AssignNewToChildLocal(ScrollBox, WorldOutlinerEntry, actorEntry, "WorldOutlinerEntry: " + actor->GetObjectName());
		actorEntry->Initialize(actor);
	}
}
void WorldOutlinerWidget::OnSelectedValuesChanged(PictorumWidget* ScrollBoxWidget, const SArray<int32>& SelectedIndices) {
	if (!SelectedIndices.IsEmpty()) {
		ReferencedWorld->SetSelectedEntity(ReferencedWorld->GetWorldActors()[SelectedIndices[0]]);
	}

	SArray<Entity*> entities;

	for (const int32& index : SelectedIndices) {
		if (index >= 0 && index <= ReferencedWorld->GetWorldActors().Count()) {
			entities.Add(ReferencedWorld->GetWorldActors()[index]);
		}
	}

	SelectedEntitiesUpdatedDelegate.Broadcast(entities);
}