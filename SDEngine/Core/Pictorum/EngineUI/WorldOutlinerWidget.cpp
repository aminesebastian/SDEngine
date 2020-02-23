#include "WorldOutlinerWidget.h"
#include "Core/Engine/World.h"
#include "Core/Objects/Entities/Actor.h"
#include "Core/Pictorum/Containers/PictorumScrollBox.h"
#include "Core/Pictorum/EngineUI/EngineUIStyle.h"
#include "Core/Pictorum/Widgets/SolidWidget.h"
#include "Core/Pictorum/Widgets/TextWidget.h"

WorldOutlinerWidget::WorldOutlinerWidget(const TString& Name) : PictorumWidget(Name) {

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
void WorldOutlinerWidget::WorldUpdated(World* WorldIn) {
	ReferencedWorld = WorldIn;
	ScrollBox->ClearChildren();
	for (Actor* actor : WorldIn->GetWorldActors()) {
		TextWidget* actorEntry = new TextWidget(actor->GetObjectName());
		actorEntry->SetText(actor->GetObjectName());
		actorEntry->SetFontSize(12);
		ScrollBox->AddChild(actorEntry);
	}
}
void WorldOutlinerWidget::OnSelectedValuesChanged(PictorumWidget* ScrollBoxWidget, const SArray<int32>& SelectedIndices) {
	if (!SelectedIndices.IsEmpty()) {
		ReferencedWorld->SetSelectedEntity(ReferencedWorld->GetWorldActors()[SelectedIndices[0]]);
	}
}