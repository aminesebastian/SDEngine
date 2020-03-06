#include "EditorModule.h"
#include "Core/Objects/Entities/Actor.h"
#include "Editor/Subsystems/EntityInspector/InspectorPanelManager.h"
#include "Editor/Subsystems/EntityInspector/Generators/ActorInspectorPanelGenerator.h"

const TString EditorModule::GetModuleName() const {
	return "Editor";
}
void EditorModule::Startup() {
	InspectorPanelManager::Get()->RegisterCustomization(TypeResolver<Actor>::Get(), new ActorInspectorPanelGenerator());
}
void EditorModule::Teardown() {

}