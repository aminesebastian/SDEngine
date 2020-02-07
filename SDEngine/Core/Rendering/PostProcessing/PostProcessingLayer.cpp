#include "PostProcessingLayer.h"
#include "UserInterface/UserInterface.h"
#include "Core/Utilities/Logger.h"


PostProcessingLayer::PostProcessingLayer(TString Name, vec2 FinalOutputDimensions) {
	LayerName = Name;
	bEnabled = true;
	RenderTargetDimensions = FinalOutputDimensions;
}
PostProcessingLayer::~PostProcessingLayer() {

}
bool PostProcessingLayer::PopulateDetailsPanel() {
	if (ImGui::Button(IsEnabled() ? "Disable" : "Enable")) {
		ToggleEnabled();
	}
	return PopulatePostProcessingDetailsPanel();
}
TString PostProcessingLayer::GetDetailsPanelName() {
	return GetPostProcessingLayerName();
}
bool PostProcessingLayer::PopulatePostProcessingDetailsPanel() {
	return true;
}
void PostProcessingLayer::Enable() {
	SD_ENGINE_INFO("Enabling Post Process Layer: {0}", GetPostProcessingLayerName())
	bEnabled = true;
}
void PostProcessingLayer::Disable() {
	SD_ENGINE_INFO("Disabling Post Process Layer: {0}", GetPostProcessingLayerName())
	bEnabled = false;
}
void PostProcessingLayer::ToggleEnabled() {
	if (IsEnabled()) {
		Disable();
	} else {
		Enable();
	}
}
bool PostProcessingLayer::IsEnabled() {
	return bEnabled;
}
TString PostProcessingLayer::GetPostProcessingLayerName() {
	return LayerName;
}
vec2 PostProcessingLayer::GetRenderTargetDimensions() {
	return RenderTargetDimensions;
}