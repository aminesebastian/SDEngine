#include "PostProcessingLayer.h"
#include "UserInterface/UserInterface.h"
#include "Core/Utilities/Logger.h"


PostProcessingLayer::PostProcessingLayer(const TString& Name, RenderViewport* OwningViewport) : OwningViewport(OwningViewport){
	LayerName = Name;
	bEnabled = true;
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
const bool& PostProcessingLayer::IsEnabled() const {
	return bEnabled;
}
const TString& PostProcessingLayer::GetPostProcessingLayerName() const {
	return LayerName;
}
const RenderViewport* PostProcessingLayer::GetOwningViewport() const {
	return OwningViewport;
}