#include "EntityInspectorWidget.h"
#include "Core/Engine/Window.h"
#include "Core/Reflection/ReflectionHelpers.h"
#include "Core/Utilities/EngineFunctionLibrary.h"
#include "Core/Utilities/Logger.h"
#include "Core/Utilities/StringUtilities.h"
#include "Editor/Subsystems/EntityInspector/InspectorPanelManager.h"
#include "Editor/UserInterface/EngineUIStyle.h"
#include "Editor/UserInterface/Inspector/TypeInspectors/VectorInspectorWidget.h"

EntityInspector::EntityInspector(const TString& Name) : PictorumWidget(Name) {
	DetailsPanelListBox = nullptr;
	DisplayedEntity = nullptr;
}
EntityInspector::~EntityInspector() {

}

void EntityInspector::OnCreated() {
	AssignNewChildLocal(SolidWidget, root, "DetailsPanelBG");
	root->SetBackgroundColor(EngineUIStyles::DARK_BACKGROUND_COLOR);
	root->SetPadding(8.0f);

	AssignNewToChildLocal(root, PictorumScrollBox, scrollBox, "ControlsScrollBox");
	AssignNewToChild(scrollBox, PictorumVerticalBox, DetailsPanelListBox, "ControlsContainer");
}
void EntityInspector::SetSelectedEntity(Entity* SelectedEntity) {
	DetailsPanelListBox->ClearChildren();

	if (!SelectedEntity) {
		return;
	}

	InspectorPanelBuilder builder(DetailsPanelListBox, SelectedEntity->StaticDescriptor(), SelectedEntity);
	IInspectorPanelGenerator* generator = InspectorPanelManager::Get()->GetGenerator(SelectedEntity->StaticDescriptor());
	generator->GenerateInspector(builder);
}
