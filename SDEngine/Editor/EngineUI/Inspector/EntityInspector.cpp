#include "EntityInspector.h"
#include "Core/Engine/Window.h"
#include "Core/Reflection/ReflectionHelpers.h"
#include "Core/Utilities/EngineFunctionLibrary.h"
#include "Core/Utilities/Logger.h"
#include "Core/Utilities/StringUtilities.h"
#include "Editor/EngineUI/EngineUIStyle.h"
#include "Editor/EngineUI/Inspector/DefaultInspectorPanelGenerator.h"
#include "Core/Pictorum/Widgets/VectorInspectorWidget.h"

EntityInspector::EntityInspector(const TString& Name) : PictorumWidget(Name) {
	DetailsPanelListBox = nullptr;
	DisplayedEntity = nullptr;
}
EntityInspector::~EntityInspector() {

}

void EntityInspector::OnCreated() {
	AssignNewChildLocal(SolidWidget, root, "DetailsPanelBG")
		root->SetBackgroundColor(EngineUIStyles::DARK_BACKGROUND_COLOR);
	root->SetPadding(8.0f);

	AssignNewToChild(root, PictorumVerticalBox, DetailsPanelListBox, "ControlsContainer")
}
void EntityInspector::SetSelectedEntity(Entity* SelectedEntity) {
	DetailsPanelListBox->ClearChildren();

	if (!SelectedEntity) {
		return;
	}

	InspectorPanelBuilder builder(DetailsPanelListBox, SelectedEntity);
	DefaultInspectorPanelGenerator generator;
	generator.GenerateInspector(builder);
}
